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
	 
	 string server = "123.56.101.64:443" ;
     string logid = "3" ;
	 string password = "123456a" ;
	 API api ;
	 api.login(server,logid,password) ;

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
   return(0);
  }
