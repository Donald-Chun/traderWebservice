//+------------------------------------------------------------------+
//|                                         MetaTrader 4 Manager API |
//|                   Copyright 2001-2012, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+

#include "MT4ManagerAPI.h"

#include "MyAPI.h"
#include <iostream>
#include <event.h>
#include <stdio.h>
using namespace std;
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int main(int argc, char* argv[])
  {
	 string server="103.40.162.86:640";
string logid="2" ;
string password="123456a";

	 API api ;
	 api.login(server,logid,password) ;

	 Sleep(1000);

	 vector<UserRecord> vc= api.getAllUserRecord() ;
	 for(int i=0;i<vc.size() ;i++){
		 cout << vc.at(i).name << endl ;
	 }

	 cout << api.getUserRecordByLogin(88889).size() << endl ;


	 /*
	 InnerManagerPump ip ;
	 ip.connect(server.c_str()) ;
	 ip.login(3,password.c_str()) ;
	 ip.switchToPump() ;
//	 WaitForSingleObject(ip.m_hEventDone, 10);
	 getchar();*/
	 
/*	   UserRecord user={0};
   user.leverage    =100;
   user.enable      =TRUE;
   user.send_reports=TRUE;
   user.user_color  =USER_COLOR_NONE;
   user.login =  0;
   strcpy(user.name ,"tt") ;
   strcpy(user.group ,"live") ;
   strcpy(user.password ,"Aewfdss") ;
   strcpy(user.email ,"11111") ;
   api.addNewUser(user) ;
   */
//	 	 api.changePassword(22434,"java2","java3",PASSWORD_TYPE::P_MASTER) ;
	  /*
	  GroupCommandInfo  gci ;
	  gci.command = GROUP_OPT::G_SETGROUP ;
	  gci.len = 1 ;
	  gci.leverage = 1000 ;
	  strcpy(gci.newgroup,"manager") ;
	  int logins[] = {7788} ;
	  api.groupOperation(gci,logins) ;
	  */
	 // api.balance(1300,22227,DEAL_TYPE::D_IN,"sdasds") ;
//	  api.credit(10000,22227,DEAL_TYPE::D_IN,"sdasds",1) ;

/*	  int total = 0;
//	  TradeRecord *rq =api.userTradesHistory(8888,1400000,1999999999,&total)  ;
	  
	  TradeRecord *r = api.ongoingTrades(8888,&total) ;
	  cout << total << endl ;
	  for(int i=0;i<total;i++){
		  cout << r[i].symbol << endl ;
		  cout << r[i].close_price << endl ;
	  }
	  cout << total << endl ;

	  */
	 vector<TradeRecord> r= api.getOngoingTradesByLogin(88888888);
	 cout << r.size() << endl ;
	 for(int i=0;i<r.size();i++){
		 cout << r[i].symbol  << endl ;
	 }


	int total =0;
   __time32_t t=1439310315 ;

   tm t1={0,0,0,24,5,115,0,0,0} ;
   tm t2={0,0,0,25,5,115,0,0,0} ;


   
   ChartInfo ci ;
   strcpy(ci.symbol,"EURUSD");
   ci.period = PERIOD_M5 ;
   ci.mode=CHART_RANGE_IN;
  // ci.end =1439654400 ;
  // ci.start = ci.end-200*5*60;
   
   ci.end =_mktime32(&t2) ;
   ci.start = _mktime32(&t1);
   int toSym = 0;
   int digit = api.symbolDetail("EURUSD").digits ;
   vector<RateInfo> ri=api.getHistoryChartData(ci,digit) ;

   for(int i=0;i<ri.size();i++){
	   cout << ri[i].high<< "\t" << ri[i].low << "\t" <<ri[i].open  << "\t" <<ri[i].close << "\t"  <<ri[i].ctm<< endl ;
   }


   cout << ci.start << endl;
   cout << ci.end << endl ;
   cout << (ci.end - ci.start)/(5*60) << endl ;
   cout << ri.size() << endl ;


   return(0);
  }
