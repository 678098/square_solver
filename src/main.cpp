#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <solver.hpp>
#include <polynome_reader.hpp>
#include <args_parser.hpp>

// FloatType for computations can be specified when configuring the project via CMake
// boost::multiprecision::cpp_dec_float_100 is very accurate but relatively slow in comparison to double
#ifdef WITH_BOOST_MULTIPRECISION
#include <boost/multiprecision/cpp_dec_float.hpp>
using FloatType = boost::multiprecision::cpp_dec_float_100;

//template Sqrt should be instantiated because we don't want to lose accuracy when using std::sqrt(double)
template<>
boost::multiprecision::cpp_dec_float_100 SqrtImpl(const boost::multiprecision::cpp_dec_float_100 &value) {
    return boost::multiprecision::sqrt(value);
}

//this template instantiation should be faster than using intermediate stringstream
template<>
boost::multiprecision::cpp_dec_float_100 ReadInteger(const char *str) {
    return boost::multiprecision::cpp_dec_float_100(str);
}
#else
using FloatType = double;
#endif


// this class holds Producer and Consumer threads and some other entities which is needed for square solving
class SquareSolverService
{
public:
    SquareSolverService(PolynomeReader<FloatType, 3> &reader, bool silent = false, int batchSize = 32):
        reader(reader),
        solver(BuildSolver<FloatType>()),
        kSilent(silent),
        kBatchSize(batchSize)
    {
    }
    virtual ~SquareSolverService() = default;
    
    void Run() {
        std::thread consumer([&](){ this->ConsumerThread(); });
        ProducerThread();
        StopConsumer();
        consumer.join();
    }

private:
    PolynomeReader<FloatType, 3> &reader;
    
    const std::unique_ptr<EquationSolver<FloatType> > solver;
    const int kBatchSize = 32;
    const bool kSilent = false;
    
    std::mutex mtx;
    std::condition_variable cvar;
    
    //we don't want to lose too much time because of the overhead of inter-thread synchronization
    //that's why we send data in batches, which are not too small and not too big
    using BatchType = std::vector<Polynome<FloatType> >;
    using BatchPtrType = std::unique_ptr<BatchType>;
    
    std::atomic<bool> batchReady = false;
    BatchPtrType preparedBatch;
    BatchPtrType formingBatch;
    
    void SendBatch() {
        std::unique_lock<std::mutex> ul(mtx);
        preparedBatch = std::move(formingBatch);
        batchReady = true;
        ul.unlock();
        cvar.notify_one();
        
        //reservation at some extent increases performance here
        formingBatch = std::make_unique<BatchType>();
        formingBatch->reserve(kBatchSize);
        
        ul.lock();
        cvar.wait(ul, [&]() { return this->batchReady.load() == false; });
    }
    
    void StopConsumer() {
        //consumer thread can be stopped via sending empty pointer to batch
        std::unique_lock<std::mutex> ul(mtx);
                        
        batchReady = true;
        ul.unlock();
        cvar.notify_one();
    }
    
    void ProducerThread() {
        formingBatch = std::make_unique<BatchType>();
        formingBatch->reserve(kBatchSize);
        
        Polynome<FloatType> poly;
        while (reader >> poly) {
            formingBatch->emplace_back(std::move(poly));

            if (formingBatch->size() >= kBatchSize) {
                SendBatch();
            }
        }
        
        //don't forget to process remaining equations
        if (!formingBatch->empty()) {
            SendBatch();
        }
    }
    
    void ConsumerThread() {
        while (true) {
            std::unique_lock<std::mutex> ul(mtx);
            cvar.wait(ul, [&]() { return this->batchReady.load(); });
            
            BatchPtrType batch = std::move(preparedBatch);
            batchReady = false;
            
            ul.unlock();
            cvar.notify_one();
            
            //sending empty batch pointer will stop the consumer
            if (!batch) break;
            
            for (const Polynome<FloatType> &poly: (*batch)) {
                auto res = solver->Solve(poly);
                //printing results in console can take too much time in comparison to all the algo
                //so it can be disabled
                if (!kSilent) {
                    std::cout << poly << " => " << res << std::endl;
                }
            }
        }
    }
};


int main(int argc, char **argv) {
    std::cout.precision(std::numeric_limits<FloatType>::digits10);
    
    //argc_offset is used to make clear from which point of arguments interesting data is placed
    int argc_offset;
    const Configuration config = ParseCmdArgs(argc, argv, argc_offset);
    
    clock_t requestStartTime = clock();
    
    //interactive mode allows to type coeffs in console in real time
    PolynomeReader<FloatType, 3> reader = config.interactive ? PolynomeReader<FloatType, 3>(std::cin) : PolynomeReader<FloatType, 3>(argc - argc_offset, argv + argc_offset);
    SquareSolverService service(reader, config.silent, config.interactive ? 1 : 32);
    service.Run();
    
    clock_t requestEndTime = clock();

    if (config.measurePerformance) {
        std::cout.precision(6);
        std::cout << "request processing time: " << (requestEndTime - requestStartTime) / double(CLOCKS_PER_SEC) << std::endl;
    }
        
    return 0;
}
