// accountThreadAtomic.cpp

#include <atomic>
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class Wallet
{
    int mMoney;
public:
    Wallet() :mMoney(0){}
    int getMoney() { return mMoney; }
    void addMoney(int money)
    {
       for(int i = 0; i < money; ++i)
       {
          mMoney++;
       }
    }
};

int testMultithreadedWallet()
{
   Wallet walletObject;
   std::vector<std::thread> threads;
   for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(&Wallet::addMoney, &walletObject, 1000));
   }
 
   for(int i = 0; i < threads.size() ; i++)
   {
       threads.at(i).join();
   }
   return walletObject.getMoney();
}
 
int main()
{
 
  int val = 0;
  bool race = false;
  while(!race)
  {
     if((val = testMultithreadedWallet()) != 5000)
     {
       std::cout << "Money in Wallet = "<<val << std::endl;
       race = true;
     }
  }
  return 0;
}