#include <iostream>
#include <thread>

/*void myPrint(){
    std::cout<<"my thread begin"<<std::endl;
    std::cout<<"my thread end"<<std::endl;
}*/

class Ta {
public:
    void operator() (){
        std::cout<<"ta"<<std::endl;
    }
};

void myPrint(const int i,const std::string &mybuf1){
    std::cout<<"my thread begin"<<std::endl;
    std::cout<<i<<std::endl;
    std::cout<<mybuf1<<std::endl;
    std::cout<<"my thread end"<<std::endl;
}

class A{
public:
    //mutable是任何时候都可以修改，包括const中
    mutable int _i;
    A(int a):_i(a){std::cout<<"A(int a)的构造函数["<<this<<"]--线程ID["<<std::this_thread::get_id()<<"]"<<std::endl;}
    A(const A& a):_i(a._i){std::cout<<"A(int a)的拷贝构造函数["<<this<<"]--线程ID["<<std::this_thread::get_id()<<"]"<<std::endl;}
    ~A(){
        std::cout<<"A析构函数["<<this<<"]--线程ID["<<std::this_thread::get_id()<<"]"<<std::endl;
    }

    void threadWork(int num){
        std::cout<<"threadwork---A["<<this<<"]--线程ID["<<std::this_thread::get_id()<<"]--num["<<num<<"]"<<std::endl;
    }

    void operator() (int num){
        std::cout<<"operator()---A["<<this<<"]--线程ID["<<std::this_thread::get_id()<<"]--num["<<num<<"]"<<std::endl;
    }
};

//这里，第2个参数，必须为const
void myPrintA(const int i,const A &myA){
    std::cout<<"my threadA begin"<<std::endl;
    std::cout<<i<<std::endl;
    std::cout<<myA._i<<std::endl;
    std::cout<<"my threadA end"<<std::endl;
}

//这里，第2个参数，不用为const了
void myPrintB(const int i,A &myA){
    myA._i = 99;
    std::cout<<"my threadB begin"<<std::endl;
    std::cout<<i<<std::endl;
    std::cout<<myA._i<<std::endl;
    std::cout<<"my threadB end"<<std::endl;
}

void myPrintC(std::unique_ptr<int> p){
    std::cout<<"my threadC begin"<<std::endl;
    std::cout<<*p<<std::endl;
    std::cout<<"my threadC end"<<std::endl;
}

int main(){
    std::cout<<"hello thread"<<std::endl;
    int i = 6;
    char mybuf[] = "this is a thread";

    Ta ta;
    std::thread myt1(ta);
    myt1.join();

    auto mylamdthread = []{
         std::cout<<"mylamdthread"<<std::endl;
    };

    std::thread myt2(mylamdthread);
    myt2.join();


    //给线程传参注意问题：
    //1、一般简单类型是用值传递
    //2、(a)如果是类class对象，用detach的话，必须用创建线程这一行构造临时对象传参，要不然会出现问题。
    //(b)线程函数中，必须用引用来接，要不然会多一次构造。
    //3、尽量不要使用detch,用detch很容易出现主线程中的内存引用问题。
    //用了临时对象后，所以的对象，包括拷贝构造都在主对象中构建起来了。
    std::thread myThread(myPrintA,i,A(i));
    //std::thread myThread(myPrint,i,std::string(mybuf));
    //myThread.join();
    myThread.detach();

     //线程ID：std::this_thread::get_id()

    //传入类对象或者智能指针
    //1、可以修改的std:ref,传入的值可以修改的。
    A b(10);
    std::thread myThread1(myPrintB,i,std::ref(b));
    myThread1.detach();

    //传入智能指针作为线程参数*************begin*********
    std::unique_ptr<int> pInt(new int(100));
    std::thread myThread2(myPrintC,std::move(pInt));
    //myThread2.detach();
    //此处必须用join
    myThread2.join();
    //传入智能指针作为线程参数*************end*********


    //*******************类成员函数作为线程参数******begin*********
    A awork(10);
    std::thread myThread3(&A::threadWork,awork,66);
    myThread3.detach();//此种写法可以用detach()

    A awork1(10);
    std::thread myThread4(&A::threadWork,&awork1,66);
    myThread4.join();//此种写法只能用join

    A awork2(10);
    std::thread myThread5(&A::threadWork,std::ref(awork2),66);
    myThread5.join();//此种写法只能用join
    //*******************类成员函数作为线程参数******end*********

    //用operator()做为线程函数
    A awork3(10);
    std::thread myThread6(std::ref(awork3),66);
    myThread6.join(); //此种写法只能用join





    //*********************************************
    std::cout<<"main thread"<<std::endl;
    while (true)
    {
        /* code */
    }

    return 0;   
}