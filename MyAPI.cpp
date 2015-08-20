
#include "MyAPI.h"



API::API(){

}

string API::_server = "" ;
string API::_login = "" ;
string API::_password = "" ;

int API::login(string server,string login,string password)
{
	int ret=RET_ERROR;
	API::_server=server ;
	API::_login = login ;
	API::_password = password ;
	if(!_manager.IsValid()) return(-1);
	if((ret=_manager->Connect(server.c_str()))!=RET_OK || 
		(ret=_manager->Login(atoi(login.c_str()),password.c_str()))!=RET_OK)
	{
		cout << "Connect to" << server.c_str() << " as "  << login.c_str() << " failed! Reason:"<< _manager->ErrorDescription(ret)<< endl ;
		return(-1);
	}
	 cout << "Connect to" << server.c_str() << " as "  << login.c_str() << " success " << endl;

		if((ret=_manager_pump->Connect(server.c_str()))!=RET_OK || 
		(ret=_manager_pump->Login(atoi(login.c_str()),password.c_str()))!=RET_OK)
	{
		cout << "Connect to" << server.c_str() << " as "  << login.c_str() << " failed! Reason:"<< _manager_pump->ErrorDescription(ret)<< endl ;
		return(-1);
	}
	 cout << "Connect to" << server.c_str() << " as "  << login.c_str() << " success " << endl;

	    if((ret=_manager_pump.switchToPump())!=RET_OK)
     {
      printf("Ex pumping switch failed (%s)\n",_manager_pump->ErrorDescription(ret));
      return(-1);
     }
	printf("Ex pumping switch successfull\n");



	int toSym = 0;
   ConSymbol * conSym=_manager->CfgRequestSymbol(&toSym) ;
   for(int i=0;i<toSym;i++){
	   cout << "loading symbol \t" << conSym[i].symbol << endl;
	   _manager._symbolsInfo.insert(pair<string,ConSymbol>(conSym[i].symbol,conSym[i]));
   }

	 return ret ;
}

vector<UserRecord> API::getAllUserRecord(){
	vector<UserRecord> ret_vec ; ;
	int total = 0;

	UserRecord * r =_manager->UsersRequest(&total);

	for(int i=0;i<total;i++){
		ret_vec.push_back(r[i]) ;
	}
	_manager->MemFree(r) ;
	return ret_vec ;
}


// in case no value
// only return  one or no UserRecord,judgement by ret.size()==0 or ret.size()==1
vector<UserRecord> API::getUserRecordByLogin(int login){
	vector<UserRecord> ret_vec ; ;
	int log[1] = {login} ;
	int total = 1;
	UserRecord * r =_manager->UserRecordsRequest(log,&total);

	if(r!=NULL){
		
	for(int i=0;i<total;i++){
		ret_vec.push_back(r[i]) ;
	}
	_manager->MemFree(r) ;
	}
	return ret_vec ;

}

int API::addNewUser(UserRecord &user)
{
   int ret=RET_ERROR;
   ret=_manager->UserRecordNew(&user);
   if(ret!=RET_OK) { 
	   cout << _manager->ErrorDescription(ret) << endl ;
   }
   return ret ;
}

int API::groupOperation(GroupCommandInfo &gci , int *logins)
{
	int ret=RET_ERROR;
	int command = gci.command ;
	ret=_manager->UsersGroupOp(&gci,logins);
	if(ret!=RET_OK) { 
	   cout << _manager->ErrorDescription(ret) << endl ;
   }
	return ret ;
}

int API::balance(double money,int login,DEAL_TYPE type,string comment)
{
	int ret=RET_ERROR;
	TradeTransInfo info={0};
	info.type   =TT_BR_BALANCE;
	info.cmd = OP_BALANCE ;
	if(type == DEAL_TYPE::D_IN ){
		info.price = money ;
	}else{
		info.price = -1.0*money ;
		//info.expiration =  0;
	}
	strcpy(info.comment,comment.c_str()) ;
	
	info.orderby = login ;
	ret=_manager->TradeTransaction(&info);
	if(ret!=RET_OK) { 
		cout << "balance failed ! Reason:" <<_manager->ErrorDescription(ret) << endl ;
    }else{
		cout << "balance call success! login:" << login << " money:"<< money << endl  ;
	}
	return ret ;
}

int API::credit(double money,int login,DEAL_TYPE type,string comment,long ExpirationTimeStamp)
{
	int ret=RET_ERROR;

	TradeTransInfo info={0};
	info.type   =TT_BR_BALANCE;
	info.cmd = OP_CREDIT ;
	if(type == DEAL_TYPE::D_IN ){
		info.price = money ;
	
	}else{
		info.price = -1.0*money ;
		//info.expiration =  0;
	}
//	clock_t now_time;
//	time ( &now_time ) ;
//	long exptime = now_time+ ExpDayGap * 24 * 60 * 60 ;
//	info.expiration = (__time32_t)exptime-_timezone ;
	info.expiration = (__time32_t)ExpirationTimeStamp-_timezone ;

	strcpy(info.comment,comment.c_str()) ;
	
	info.orderby = login ;
	ret=_manager->TradeTransaction(&info);
	if(ret!=RET_OK) { 
		cout << "credit failed ! Reason:" <<_manager->ErrorDescription(ret) << endl ;
    }else{
		cout << "credit call success! login:" << login << " money:"<< money << endl  ;
	}
	return ret ;
}

vector<TradeRecord> API::getTradesHistoryByLogin(int login,long from,long to){
	vector<TradeRecord> ret_vec ; ;
	int total = 0;
	TradeRecord * r =_manager->TradesUserHistory(login,from, to,&total);
	for(int i=0;i<total;i++){
		ret_vec.push_back(r[i]) ;
	}
	_manager->MemFree(r) ;
	return ret_vec ;

}

vector<TradeRecord> API::getOngoingTradesByLogin(int login){

	vector<TradeRecord> ret_vec ; 
	int total = 0;
	vector<UserRecord> ru=this->getUserRecordByLogin(login) ;
	if(ru.size() == 0)
		return ret_vec;
	string group=ru.at(0).group ;
	TradeRecord * r =_manager_pump->TradesGetByLogin(login,group.c_str(),&total);
	for(int i=0;i<total;i++){
		if(r[i].login == login)
			ret_vec.push_back(r[i]) ;
	}
	_manager->MemFree(r) ;

	return ret_vec ;
	

}

vector<TradeRecord> API::getAllOngoingTrades(){
	vector<TradeRecord> ret_vec ; 
	int total = 0;
	TradeRecord * r =_manager->TradesRequest(&total);
	for(int i=0;i<total;i++){
		ret_vec.push_back(r[i]) ;
	}
	_manager->MemFree(r) ;
	return ret_vec ;
}
int API::changePassword(int login,string master_password,string new_password,PASSWORD_TYPE p_type){
	int ret=RET_ERROR;
	ret=_manager->UserPasswordCheck(login,master_password.c_str()) ;
	if(ret!=RET_OK) { 
		cout << "check failed! Reason:" <<_manager->ErrorDescription(ret) << endl ;
		return ret ;
    }else{
		cout << "check success" << login << endl  ;
	}
	
	
	if(p_type == PASSWORD_TYPE::P_MASTER){
		ret =_manager->UserPasswordSet(login,new_password.c_str(),0,0) ;
			if(ret!=RET_OK) { 
			cout << "set master password failed! Reason:" <<_manager->ErrorDescription(ret) << endl ;
			return ret ;
		}else{
			cout << "set master password success" << login << endl  ;
		}
	}else{
		ret =_manager->UserPasswordSet(login,new_password.c_str(),1,0) ;
			if(ret!=RET_OK) { 
			cout << "set invest password failed:" <<_manager->ErrorDescription(ret) << endl ;
			return ret ;
		}else{
			cout << "set invest password success:" << login << endl  ;
		}
	
	}
	return ret ;
	
}



vector<RateInfo> API::getHistoryChartData(ChartInfo &ci,int &digit){
	vector<RateInfo> ret_vec ; 
	__time32_t t ;
	int total = 0;
	
	
	RateInfo * r=NULL ;
	try{
		r=_manager->ChartRequest(&ci,&t,&total);
	}catch(exception &e){
		cout << e.what() << endl ;
	}
	int digits = this->symbolDetail(ci.symbol).digits ;
	digit=digits;
	
	for(int i=total-1;i>=0;i--){
		r[i].close = r[i].open + r[i].close ;
		r[i].high = r[i].open + r[i].high ;
		r[i].low = r[i].open + r[i].low ;
		r[i].open = r[i].open  ;
		//cout << "DEBUG: API::getHistoryChartData  time:" <<r[i].ctm << endl ;
		ret_vec.push_back(r[i]) ;
		
	}
	_manager->MemFree(r) ;
	return ret_vec ;
}


ConSymbol API::symbolDetail(string symbol){
	ConSymbol ret={} ;
	map<string,ConSymbol>::iterator  m_iter =_manager._symbolsInfo.find(symbol) ;
	if(m_iter==_manager._symbolsInfo.end())
		return ret ;
	ret=m_iter->second ;
	//_manager->SymbolGet(symbol.c_str(),&ret);
   return ret ;
}


vector<ConGroup> API::allGroups(){
	int num=0;
	vector<ConGroup> ret_vec ;
	ConGroup * ret =_manager->CfgRequestGroup(&num) ;
	for(int i=0;i<num;i++){
		ret_vec.push_back(ret[i]) ;
	}
	_manager->MemFree(ret) ;
	return ret_vec ;
}