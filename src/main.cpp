#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

#include <polynome.hpp>
#include <solver.hpp>
#include <polynome_reader.hpp>
#include <args_parser.hpp>

using FloatType = double;

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
    std::atomic<bool> batchReady = false;
    std::mutex mtx;
    std::condition_variable cvar;
    std::vector<Polynome<FloatType> > formingBatch;
    std::vector<Polynome<FloatType> > preparedBatch;
    
    const int kBatchSize = 32;
    const bool kSilent = false;
    
    void ProcessBatch() {
        std::unique_lock<std::mutex> ul(mtx);
        preparedBatch = std::move(formingBatch);
        batchReady = true;
        ul.unlock();
        cvar.notify_one();
        
        formingBatch.clear();
        formingBatch.reserve(kBatchSize);
        
        ul.lock();
        cvar.wait(ul, [&]() { return this->batchReady.load() == false; });
    }
    
    void StopConsumer() {
        std::unique_lock<std::mutex> ul(mtx);
                        
        batchReady = true;
        ul.unlock();
        cvar.notify_one();
    }
    
    void ProducerThread() {
        Polynome<FloatType> poly;
        while (reader >> poly) {
            formingBatch.emplace_back(std::move(poly));

            if (formingBatch.size() >= kBatchSize) {
                ProcessBatch();
            }
        }
        
        if (!formingBatch.empty()) {
            ProcessBatch();
        }
    }
    
    void ConsumerThread() {
        while (true) {
            std::unique_lock<std::mutex> ul(mtx);
            cvar.wait(ul, [&]() { return this->batchReady.load(); });
            
            std::vector<Polynome<FloatType> > dt = std::move(preparedBatch);
            batchReady = false;
            
            ul.unlock();
            cvar.notify_one();
            
            if (dt.empty()) break;
            
            for (const Polynome<FloatType> &poly: dt) {
                auto res = solver->Solve(poly);
                if (!kSilent) {
                    std::cout << poly << " => " << res << std::endl;
                }
            }
        }
    }
};

int main(int argc, char **argv) {
    int argc_offset;
    const Configuration config = ParseCmdArgs(argc, argv, argc_offset);
    
    clock_t requestStartTime = clock();
    
    PolynomeReader<FloatType, 3> reader = config.interactive ? PolynomeReader<FloatType, 3>(std::cin) : PolynomeReader<FloatType, 3>(argc - argc_offset, argv + argc_offset);
    SquareSolverService service(reader, config.silent, config.interactive ? 1 : 32);
    service.Run();
    
    clock_t requestEndTime = clock();

    if (config.measurePerformance) {
        std::cout << "request processing time: " << (requestEndTime - requestStartTime) / double(CLOCKS_PER_SEC) << std::endl;
    }
        
    return 0;
}
