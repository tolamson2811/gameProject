#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void paused();
    void unpaused();    

    int get_time();

    bool is_started();
    bool is_paused();
private:
    int start_time_; //Thời điểm bắt đầu chạy chương trình 
    int paused_time_;// Thời điểm tạm dừng chương trình

    bool is_paused_;
    bool is_started_;
};


#endif