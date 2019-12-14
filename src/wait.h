#ifndef WAIT_H
#define WAIT_H
	
	#include <memory>
    #include <condition_variable>
    #include <mutex>

    //clase para detener y reanudar hilos
    class Wait{
        private:
            std::mutex mutex;
            std::shared_ptr<std::condition_variable> event;
        public:
            //constructor
            Wait();
            //detiene el hilo
            void wait();
            //activa los hilos detenidos
            void notify_all();
    };

#endif
