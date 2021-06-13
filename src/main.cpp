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


class SquareSolverService
{
public:
    SquareSolverService(bool silent = false):
        solver(BuildSolver<double>()),
        kSilent(silent)
    {
    }
    virtual ~SquareSolverService() = default;
    
    //todo hide argc/argv in PolynomeReader
    void Run(int argc, char **argv) {
        std::thread consumer([&](){ this->ConsumerThread(); });
        ProducerThread(argc, argv);
        StopConsumer();
        consumer.join();
    }

private:
    const std::unique_ptr<EquationSolver<double> > solver;
    std::atomic<bool> batchReady = false;
    std::mutex mtx;
    std::condition_variable cvar;
    std::vector<Polynome<double> > formingBatch;
    std::vector<Polynome<double> > preparedBatch;
    
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
    
    void ProducerThread(int argc, char **argv) {
        for (int i = 0; i < 100; i++) {
            PolynomeReader<double, 3> reader(argc, argv);
            Polynome<double> poly;
            while (reader >> poly) {
                formingBatch.emplace_back(std::move(poly));

                if (formingBatch.size() > kBatchSize) {
                    ProcessBatch();
                }
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
            
            if (preparedBatch.empty()) break;
            
            std::vector<Polynome<double> > dt = std::move(preparedBatch);
            batchReady = false;
            
            ul.unlock();
            cvar.notify_one();
            
            for (const Polynome<double> &poly: dt) {
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
    
    SquareSolverService service(config.silent);
    service.Run(argc - argc_offset, argv + argc_offset);
    
    clock_t requestEndTime = clock();

    if (config.measurePerformance) {
        std::cout << "request processing time: " << (requestEndTime - requestStartTime) / double(CLOCKS_PER_SEC) << std::endl;
    }
        
    return 0;
}
