#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

#include <polynome.hpp>
#include <solver.hpp>
#include <polynome_reader.hpp>


class SquareSolverService
{
public:
    SquareSolverService(): solver(BuildSolver<double>()) {
    }
    virtual ~SquareSolverService() = default;
    
    //todo hide argc/argv in PolynomeReader
    void Run(int argc, char **argv) {
        std::thread consumer([&](){ this->ConsumerThread(); });
        ProducerThread(argc, argv);
        Finalize();
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
    
    
    void OnPolynomeRead(const Polynome<double> &poly) {
        formingBatch.emplace_back(std::move(poly));

        if (formingBatch.size() > kBatchSize) {
            ProcessBatch();
        }
    }
    
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
    
    void Finalize() {
        if (!formingBatch.empty()) {
            ProcessBatch();
        }
        
        std::unique_lock<std::mutex> ul(mtx);
                        
        batchReady = true;
        ul.unlock();
        cvar.notify_one();
    }
    
    void ProducerThread(int argc, char **argv) {
        PolynomeReader<double, 3> reader;
        for (int i = 0; i < 100; i++) {
            reader.Read(argc, argv, [&](Polynome<double> poly) {
                this->OnPolynomeRead(poly);
            });
        }
        Finalize();
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
                std::cout << poly << " => " << res << std::endl;
            }
        }
    }
};

int main(int argc, char **argv) {
    
    clock_t requestStartTime = clock();
    
    SquareSolverService service;
    service.Run(argc - 1, argv + 1);
    
    clock_t requestEndTime = clock();
    //todo get rid of this in the final version
    std::cout << "Request processing time: " << (requestEndTime - requestStartTime) / double(CLOCKS_PER_SEC) << std::endl;
        
    return 0;
}
