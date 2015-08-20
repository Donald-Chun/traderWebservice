#pragma once
#define WINVER         0x0501
#define _WIN32_IE      0x0600
#include "MT4ManagerAPI.h"
#include <Winsock2.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <process.h>
#include <windows.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

class InnerManager
  {
private:
   CManagerFactory   m_factory;
   CManagerInterface *m_manager;


public:
	map<string,ConSymbol> _symbolsInfo ;
   InnerManager() : m_factory("mtmanapi.dll"),m_manager(NULL)
     {
      m_factory.WinsockStartup();
      if(m_factory.IsValid()==FALSE || (m_manager=m_factory.Create(ManAPIVersion))==NULL)
        {
         printf("Failed to create MetaTrader 4 Manager API interface\n");
         return;
        }
     }

   ~InnerManager()
     {
      if(m_manager!=NULL)
        {
         if(m_manager->IsConnected())
            m_manager->Disconnect();
         m_manager->Release();
         m_manager=NULL;
        }
      m_factory.WinsockCleanup();
     }

   bool IsValid()
     {
      return(m_manager!=NULL);
     }

   CManagerInterface* operator->()
     {
      return(m_manager);
     }
  };


class InnerManagerPump{
public:
    CManagerInterface* m_manager;
	CManagerFactory   m_factory;
    HANDLE m_hEventDone;
public:
 
     InnerManagerPump(): m_factory("mtmanapi.dll"),m_manager(NULL){
      m_factory.WinsockStartup();
      if(m_factory.IsValid()==FALSE || (m_manager=m_factory.Create(ManAPIVersion))==NULL)
        {
         printf("Failed to create MetaTrader 4 Manager Pump API interface\n");
         return;
        }
     }

     ~InnerManagerPump(){
     if(m_manager!=NULL)
        {
         if(m_manager->IsConnected())
            m_manager->Disconnect();
         m_manager->Release();
         m_manager=NULL;
        }
      m_factory.WinsockCleanup();
     }
	     int switchToPump(){
      return m_manager->PumpingSwitchEx(NotifyCallBack, 0, this);
    }
	 CManagerInterface* operator->()
     {
      return(m_manager);
     }


private:
    //This function is a callback that will be called by the pumping thread and will call the appropriate member methods of the class (like a switchboard)
    static void __stdcall NotifyCallBack(int code,int type,void* data,void *param){
          InnerManagerPump* pThis = (InnerManagerPump*) param;
    }
};

enum GROUP_OPT{ 
	G_DELETE=0,
	G_ENABLE,
	G_DISABLE,
	G_LEVERAGE,
	G_SETGROUP };

enum DEAL_TYPE {
	D_IN,
	D_OUT
	};

enum PASSWORD_TYPE{
	P_MASTER ,
	P_INVESTER,
};

class API{

private:
	InnerManager _manager ;
	InnerManagerPump _manager_pump ;
	
	static string _server, _login, _password ;
	bool _loginFlag  ;

    static void __stdcall NotifyCallBack(int code,int type,void* data,void *param){

    }
public:
	API() ;
	vector<UserRecord> getAllUserRecord() ;
	vector<UserRecord> getUserRecordByLogin(int login) ;
	int addNewUser(UserRecord &user) ; 
	int login(string server,string login,string password) ;
	int groupOperation(GroupCommandInfo &gci , int *logins) ;
	int balance(double money,int login,DEAL_TYPE type,string comment) ;
	int credit(double money,int login,DEAL_TYPE type,string comment,long ExpirationTimeStamp) ;
	vector<TradeRecord> getTradesHistoryByLogin(int login,long fromTimeStamp, long toTimeStamp);
	vector<TradeRecord> getOngoingTradesByLogin(int login) ;
	vector<TradeRecord> getAllOngoingTrades() ;
	int changePassword(int login,string master_password,string new_password,PASSWORD_TYPE p_type) ;
	//====================================

	vector<RateInfo> getHistoryChartData(ChartInfo &ci,int &digit) ;

	ConSymbol symbolDetail(string symbol) ;

	//====================================
	vector<ConGroup> allGroups() ;


	static void  __stdcall TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired){
		int ret=(*((InnerManager *)lpParam))->Ping() ;
		int count = 3;
		if(ret==RET_OK)
			cout << "Auto ping mt4 origin server. Status: OK" << endl ;
		else{
			while(ret!=RET_OK && count != 0 ){
			cout << "Auto ping mt4 origin server. Status: Out of Connection !!!" << endl ;
			cout << "try connecting ...." << endl ;
			InnerManager  & _m = (*((InnerManager *)lpParam)) ;
			if((ret=_m->Connect(API::_server.c_str()))!=RET_OK || 
				(ret=_m->Login(atoi(API::_login.c_str()),API::_password.c_str()))!=RET_OK)
			{
			cout << "Connect to" << API::_server.c_str() << " as "  << API::_login.c_str() << " failed! Reason:"<< _m->ErrorDescription(ret)<< endl ;			
			}else{
				cout << "Connect to" << API::_server.c_str() << " as "  << API::_login.c_str() << " success " << endl;
					int toSym = 0;
					ConSymbol * conSym=_m->CfgRequestSymbol(&toSym) ;
					for(int i=0;i<toSym;i++){
						 cout << "loading symbol \t" << conSym[i].symbol << endl;
							_m._symbolsInfo.insert(pair<string,ConSymbol>(conSym[i].symbol,conSym[i]));
					 }
			}
			Sleep(8) ;
			count-- ;
			}
		}
	}

		static void  __stdcall TimerRoutinePump(PVOID lpParam, BOOLEAN TimerOrWaitFired){
		int ret=(*((InnerManagerPump *)lpParam))->Ping() ;
		int count = 3;
		if(ret==RET_OK)
			cout << "Pump Auto ping mt4 origin server. Status: OK" << endl ;
		else{
			while(ret!=RET_OK && count != 0 ){
			cout << "Pump Auto ping mt4 origin server. Status: Out of Connection !!!" << endl ;
			cout << "try connecting pump ...." << endl ;
			InnerManagerPump  & _manager_pump = (*((InnerManagerPump *)lpParam)) ;
			if((ret=_manager_pump->Connect(API::_server.c_str()))!=RET_OK || 
						(ret=_manager_pump->Login(atoi(API::_login.c_str()),API::_password.c_str()))!=RET_OK)
						{
							cout << "Connect to" << API::_server.c_str() << " as "  << API::_login.c_str() << " failed! Reason:"<< _manager_pump->ErrorDescription(ret)<< endl ;
						}else{
					 cout << "Connect to" << API::_server.c_str() << " as "  << API::_login.c_str() << " success " << endl;
					}

			if((ret=_manager_pump.switchToPump())!=RET_OK)
			{
				printf("Ex pumping switch failed (%s)\n",_manager_pump->ErrorDescription(ret));
			}else{
				printf("Ex pumping switch successfull\n");
			}
			Sleep(8) ;
			count-- ;
			}
		}
	}



	int continueConnection(){
		HANDLE hTimerQueue=NULL;
		HANDLE hTimer=NULL;
		 hTimerQueue = CreateTimerQueue();
		if (NULL == hTimerQueue) {
        printf("CreateTimerQueue failed (%d)\n", GetLastError());
        return -1;
		} 
		if (!CreateTimerQueueTimer( &hTimer, hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutine, &_manager, 0, 180000, 0))	{
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		} 
		return 0 ;
	}
	int continuePumpConnection(){
		HANDLE hTimerQueue=NULL;
		HANDLE hTimer=NULL;
		hTimerQueue = CreateTimerQueue();
		if (NULL == hTimerQueue) {
        printf("CreateTimerQueue failed (%d)\n", GetLastError());
        return -1;
		} 
		if (!CreateTimerQueueTimer( &hTimer, hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutinePump, &_manager_pump, 0, 180000, 0))	{
        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		} 
		return 0 ;
	}

} ;