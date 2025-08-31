#pragma once


namespace smart_home::utilities {

    template <typename T>
    class Singleton {
    private:
        inline static T* activeInstance = nullptr;
    protected:
        static void setInstance(T* newInstance) {
            activeInstance = newInstance;
        }
    public:
        static T* getInstance() {
            return activeInstance;
        }
    };
} // namespace smart_home::utilities