#ifndef CUSTOM_FUNCTION_H
#define CUSTOM_FUNCTION_H
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <time.h>
using namespace std;

class Custom_Function
{
private:
    /* data */

public:
Custom_Function(){

}
//去除字符串中的所有空格
void Custom_trim(string &s)
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
}

//由d_Component更新Component，比如由d_Machine_Component更新Machine_Component
map<string,int> Custom_update_Component(map<string,int> d_component,map<string,int> component){
    map<string, int>::iterator  iter;
    for(iter = component.begin(); iter != component.end(); iter++)
    {
        component[iter->first] += d_component[iter->first];
    }
    return component;
}

//创建键与component一致，值全为0的d_Component
map<string,int> Custom_get_d_Component(map<string,int> component){
    map<string,int> d_Component = component;
    map<string,int>::iterator  iter;
    for(iter = d_Component.begin(); iter != d_Component.end(); iter++)
    {
        d_Component[iter->first] = 0;
    }
    return d_Component;
}

//由单个不等式表达式得到属于作业几的任务
string Custom_get_Assignment_Name(string expression){
    return "Assignment"+Custom_get_split(expression,"_")[1];
}

//由不等式表达式得value值
int Custom_get_value(string str,map<string,int>component){
    int res = 0;
    string name = Custom_get_split(str,">")[0];
    res = component[name];
    return res;
}

//由不等式表达式得到机械臂/AGV此次搬运的物料类型及数量
map<string,int> Custom_get_hold(string str){
    map<string,int> res;

    vector<string> temp = Custom_get_split(str,"or");
	if (temp.size() > 0) {
		for (int i = 0; i < temp.size(); i++) {
			string name = Custom_get_split(temp[i], ">")[0];
			res[name] = 0;
		}
	}

    return res;
}

//由单个不等式表达式(比如Warehouse1_1_0_Num>0)得到实体名称
string get_entities_name(string s){
    return Custom_get_split(Custom_get_split(s,">")[0],"_")[0];
}

//将字符串s按照字符串c进行拆分
vector<string> Custom_get_split(string s,  string c)
{
    vector<string> v;
    string::size_type pos1 = 0, pos2 = s.find(c);
    while (string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));
    
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
    v.push_back(s.substr(pos1));
    return v;
}

//判断单个不等式是否成立，比如Machin1_1_0_Num>0是否成立
bool Custom_judge(string str,map<string,int> component){
    bool res = false;
    vector<string> name_splits = Custom_get_split(str,">");  
	if (name_splits.size() > 0) {
		string Component_Name = name_splits[0];
		int Component_Num = component[Component_Name];
		if (Component_Num > stoi(name_splits[1])) {
			res = true;
		}
	}
    return res;
}


//查找双层字典中的第二个key是否为某字符串,若是则根据value值更新component
void check_string_in_doublemap_and_upadte(string s,map<string,map<string,int>>& component,int value){

    map<string,map<string,int> >::iterator muliter;  
    map<string,int>::iterator iter;
    for(muliter=component.begin();muliter!=component.end();muliter++)
    {
        map<string,int>::iterator temp;
        temp = muliter->second.find(s);
        if(temp != muliter->second.end())
            component[muliter->first][s] += value;
		//else if (s.find("Warehouse") != string::npos and s.find("Rest") == string::npos) {
		//	component[muliter->first][s] = value;
		//}
    }
}

//保留三位小数
double setPrecision(double x){
    for(int i = 0; i < 4 ; i++)
        x = x * 10;
    //cout << "x " << x << endl;
    int carry = (int)x % 10;
    //cout << "carry " << carry <<endl;
    x = x / 10;
    //cout << "x " << x << endl;
    int res = (int) x;
    //cout << "res " << res << endl;
    if(carry >= 5){
         res = res + 1;
    }

    //cout << "res " << res << endl;
    return (double)res / pow(10,3);
}

//保留n位小数
double setPrecision(double x,int n)
{
    for(int i = 0; i < n + 1 ; i++)
        x = x * 10;
    //cout << "x " << x << endl;
    int carry = (int)x % 10;
    //cout << "carry " << carry <<endl;
    x = x / 10;
    //cout << "x " << x << endl;
    int res = (int) x;
   // cout << "res " << res << endl;
    if(carry >= 5)
    {
        res = res + 1;
    }

    //cout << "res " << res << endl;
   //cout << "result" <<  (double)res / pow(10,n) << endl;
    return (double)res / pow(10,n);
}


//查找单层字典中的key是否为某字符串,若是则根据value值更新component
void check_string_in_map_and_upadte(string s,map<string,int>& component,int value){

    map<string,int>::iterator iter;
    iter = component.find(s);
    if(iter != component.end())
        component[s] += value;
}



//由AGV/机械臂判断flag得到出发地，目的地
void get_start_end_position(string& StartPosition,string& EndPosition,string flag,int i){

    //flag:
    //AGV1_Rest>0andWarehouse4_1_0_Num>0orWarehouse4_2_0_Num>0orWarehouse4_3_0_Num>0andWarehouse1_Rest>0
    vector<string> f;
    if(flag.find("||") != string :: npos){
        f = Custom_get_split(flag,"||"); 
    }else
        f.push_back(flag); 

    string satisfy_flag = f[i];

    vector<string> partflag = Custom_get_split(satisfy_flag,"and");

    StartPosition = Custom_get_split(partflag[1],"_")[0];
    EndPosition = Custom_get_split(partflag[2],"_")[0];
}


/*
机器
*/
//判断机器是否满足从Idle到Loading的条件，若满足则返回true，否则返回false
bool Custom_get_Machine_bool(string Machine_Flag, map<string,int> Machine_Component,int& Machine_i){
	Custom_trim(Machine_Flag);   //去除字符串中的空格
    bool res = false;
    vector<string> flag;
    //1.对Machine_Flag进行拆分 
    //1.1判断Machine_Flag是否可以进行拆分
    if(Machine_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Machine_Flag,"or"); 
    }else
        flag.push_back(Machine_Flag); 
		

    for(int i = 0; i < flag.size(); i++){
        if(Custom_judge(flag[i],Machine_Component)){
            res = true;
            Machine_i = i;
            break;
        }
    }
    return res;
}

//机器从Loading到Processing这个变迁上执行的动作
void Custom_Machine_Loading_Processing_Action(map<string,map<string,double>> Material_Distribution_Value,map<string,string> Material_Distribution_Name,string& Material_Assignment_Name,int Machine_n,string Machine_Flag, map<string,int> Machine_Component, int Machine_i, map<string,double> Machine_ProcessTime, double t, map<string,int>& Material_Num, double& t_next,map<string,int>& d_Machine_Component){
    vector<string> flag;
	Custom_trim(Machine_Flag);   //去除字符串中的空格

    //1.对Machine_Flag进行拆分 
    //1.1判断Machine_Flag是否可以进行拆分
    if(Machine_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Machine_Flag,"or"); 
    }else
        flag.push_back(Machine_Flag); 
    //2.由Machine_i得到满足条件的判断语句
    string satisfy_flag = flag[Machine_i];
    //2.1 得到当前机器正在加工作业几
    Material_Assignment_Name = Custom_get_Assignment_Name(satisfy_flag);
    //3.对flag[Machine_i]再次进行拆分，得到原材料id
    vector<string> name_splits = Custom_get_split(satisfy_flag,">");  //name_splits[0]:零件id name_splits[1]:>右侧的个数
    string material = name_splits[0];  //零件id，比如Machine1_1_0_Num
    double processTime = Machine_ProcessTime[material];   //由原材料id得到其加工时间
    if(processTime == -1.0){  //说明用户设计的是一个随机分布而不是具体的加工时间
        string distribution_name = Material_Distribution_Name[material];   //取得随机分布的名称
        map<string,map<string,double>> temp;
        temp[distribution_name] = Material_Distribution_Value[material];  //Material_Distribution_Value存储用户随机分布每个变量名称及其值
        processTime = calculateTime(distribution_name,temp);
    }
    t_next = t + processTime;
    int process_num = min(Machine_n,Machine_Component[material]);  //得到当前加工原材料的个
    Material_Num[material] = process_num;
    d_Machine_Component[material] -= process_num;
    
}

//机器从Uploading到Idle这个变迁上执行的动作
void Custom_Machine_Uploading_Idle_Action(string& Material_Assignment_Name,string Machine_Flag,  int& Machine_i, map<string,int> Material_Num, map<string,string> Material_Product_Name,map<string,int>& d_Machine_Component){
    vector<string> flag;

	Custom_trim(Machine_Flag);   //去除字符串中的空格

    //1.对Machine_Flag进行拆分 
    //1.1判断Machine_Flag是否可以进行拆分
    if(Machine_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Machine_Flag,"or"); 
    }else
        flag.push_back(Machine_Flag); 
    //2.取得满足条件的不等式表达式flag,并将flag进行拆分，得到原材料
    vector<string> name_splits = Custom_get_split(flag[Machine_i],">");  
    string material = name_splits[0];  
    //3.由原材料取得对应的产品
    string product = Material_Product_Name[material];
    //4.由此次加工原材料个数取得产品个数
    int process_num = Material_Num[material];
    d_Machine_Component[product] += process_num;
    //重置
    Machine_i = -1;
    Material_Assignment_Name = "";
}

/*
组合机器
*/

//判断组合机器单个不等式表达式是否成立  Machine1_1_0_Num>=n_1_0_Num
bool Custom_Combiner_judge(string s,map<string,int> Combiner_Component,map<string,int> Combiner_n){
    bool res = false;
    vector<string> temp = Custom_get_split(s,">=");
    int value1 = Combiner_Component[temp[0]];
    int value2 = Combiner_n[temp[1]];

    if(value1>=value2)
        res = true;

    return res;
}
//组合机器判断多个表达式是否同时成立
bool Custom_Combiner_Multi_judge(vector<bool> recording){
    bool res = true;
    for(int i=0; i<recording.size(); i++){
        if(!recording[i]){
            res = false;
            break;
        }
    }
    return res;
}
//判断组合机器是否满足从Idle到Loading的条件，若满足则返回true，否则返回false
bool Custom_get_Combiner_bool(string Combiner_Flag, map<string,int> Combiner_Component,map<int,map<string,int>> Combiner_n,int& Combiner_i){
    bool res = false;
    vector<string> flag;
	Custom_trim(Combiner_Flag);   //去除字符串中的空格
    //1.对Combiner_Flag进行拆分 
    //1.1判断Combiner_Flag是否可以进行拆分
    if(Combiner_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Combiner_Flag,"or"); 
    }else
        flag.push_back(Combiner_Flag); 

    for(int i = 0; i < flag.size(); i++){  //Machine1_1_0_Num>=n_1_0_NumandMachine1_1_1>=n_1_1_NumandMachine1_1_2>=n_1_2_Num
        vector<string> partflag = Custom_get_split(flag[i],"and");
        vector<bool> recording;
        for(int j = 0;j < partflag.size();j++){ //Machine1_1_0_Num>=n_1_0_Num、Machine1_1_1>=n_1_1_Num、Machine1_1_2>=n_1_2_Num
            recording.push_back(Custom_Combiner_judge(partflag[j],Combiner_Component,Combiner_n[i]));
        }
		if (recording.size() > 0) {  //防止遍历vetcor下标越界
			if (Custom_Combiner_Multi_judge(recording)) {
				Combiner_i = i;
				res = true;
				break;
			}
		}

    }
    return res;
}
//组合机器从Loading到Processing这个变迁上执行的动作
void Custom_Combiner_Loading_Processing_Action(map<string,map<string,double>> Material_Distribution_Value,map<string,string> Material_Distribution_Name,string& Material_Assignment_Name,string Combiner_Flag, map<string,int> Combiner_Component,int Combiner_i,map<string,double> Combiner_ProcessTime, double t, map<int,map<string,int>> Combiner_n, double& t_next,map<string,int>& d_Combiner_Component){
    vector<string> flag;
	Custom_trim(Combiner_Flag);   //去除字符串中的空格
    //1.对Combiner_Flag进行拆分 
    //1.1判断Combiner_Flag是否可以进行拆分
    if(Combiner_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Combiner_Flag,"or"); 
    }else
        flag.push_back(Combiner_Flag); 
    //2.由Combiner_i得到满足条件的判断语句
    string satisfy_flag = flag[Combiner_i];
    vector<string> partflag = Custom_get_split(satisfy_flag,"and");
    //2.1 得到当前组合机器正在加工作业几
    Material_Assignment_Name = Custom_get_Assignment_Name(partflag[0]);
    //2.2 得到组合加工零件名称拼接字符串 比如:Machine1_1_0_NumMachine1_1_1_NumMachine1_1_2_Num
    string combiner_name = "";
    for(int i=0;i<partflag.size();i++){
        combiner_name += Custom_get_split(partflag[i],">=")[0];
    }
    //2.3 由组合零件名称得到加工时间
    double processTime = Combiner_ProcessTime[combiner_name];
    if(processTime == -1){ //说明用户设计的是一个随机分布而不是具体的加工时间
        string distribution_name = Material_Distribution_Name[combiner_name];  //取得随机分布的名称
        map<string,map<string,double>> temp;
        temp[distribution_name] = Material_Distribution_Value[combiner_name];
        processTime = calculateTime(distribution_name,temp);
    }
    //2.4 更新t_next
    t_next = t + processTime;
    //2.5 对组合零件的各个值进行减操作
    for(int i=0;i<partflag.size();i++){
        vector<string> temp = Custom_get_split(partflag[i],">=");
        string name = temp[0];
        int num = Combiner_n[Combiner_i][temp[1]];
        d_Combiner_Component[name]-=num;
    }

}
//组合机器从Uploading到Idle这个变迁上执行的动作
void Custom_Combiner_Uploading_Idle_Action(string& Material_Assignment_Name,string Combiner_Flag,map<string,int> Combiner_Component,map<string,string> Material_Product_Name,int Combiner_i,map<string,int>& d_Combiner_Component){
    vector<string> flag;
	Custom_trim(Combiner_Flag);   //去除字符串中的空格
    //1.对Machine_Flag进行拆分 
    //1.1判断Machine_Flag是否可以进行拆分
    if(Combiner_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Combiner_Flag,"or"); 
    }else
        flag.push_back(Combiner_Flag); 
    //2.1 由Combiner_i得到满足条件的判断语句
    string satisfy_flag = flag[Combiner_i];
    vector<string> partflag = Custom_get_split(satisfy_flag,"and");
    //2.2 得到组合加工零件名称拼接字符串 比如:Machine1_1_0_NumMachine1_1_1_NumMachine1_1_2_Num
    string combiner_name = "";
    for(int i=0;i<partflag.size();i++){
        combiner_name += Custom_get_split(partflag[i],">=")[0];
    }
    //2.2 得到产品名称
    string product = Material_Product_Name[combiner_name];

    d_Combiner_Component[product] += 1;
    //重置
    Combiner_i = -1;
    Material_Assignment_Name = "";
}

/*
拆分机器
*/
//判断拆分机器是否满足从Idle到Loading的条件，若满足则返回true，否则返回false
bool Custom_get_Splitting_bool(string Splitting_Flag,map<string,int> Splitting_Component,int& Splitting_i){
 bool res = false;
    vector<string> flag;
	Custom_trim(Splitting_Flag);   //去除字符串中的空格
    //1.对Splitting_Flag进行拆分 
    //1.1判断Splitting_Flag是否可以进行拆分
    if(Splitting_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Splitting_Flag,"or"); 
    }else
        flag.push_back(Splitting_Flag); 

    for(int i = 0; i < flag.size(); i++){
        if(Custom_judge(flag[i],Splitting_Component)){
            res = true;
            Splitting_i = i;
            break;
        }
    }
    return res;
}
//拆分机器从Loading到Processing这个变迁上执行的动作
void Custom_Splitting_Loading_Processing_Action(map<string,map<string,double>> Material_Distribution_Value,map<string,string> Material_Distribution_Name,map<string,double> Splitting_ProcessTime,string& Material_Assignment_Name,string Splitting_Flag,map<string,int> Splitting_Component,int Splitting_i,double t,double& t_next,map<string,int> d_Splitting_Component){
    vector<string> flag;
	Custom_trim(Splitting_Flag);   //去除字符串中的空格
    //1.对Splitting_Flag进行拆分 
    //1.1判断Splitting_Flag是否可以进行拆分
    if(Splitting_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Splitting_Flag,"or"); 
    }else
        flag.push_back(Splitting_Flag); 
    //2.由Machine_i得到满足条件的判断语句
    string satisfy_flag = flag[Splitting_i];
    //2.1 得到当前机器正在加工作业几
    Material_Assignment_Name = Custom_get_Assignment_Name(satisfy_flag);
    //2.2 得到拆分零件的名称
    string splitting_name = Custom_get_split(satisfy_flag,">")[0];
    double processTime = Splitting_ProcessTime[splitting_name];
    if(processTime == -1){
        string distribution_name = Material_Distribution_Name[splitting_name];
        map<string,map<string,double>> temp;
        temp[distribution_name] = Material_Distribution_Value[splitting_name];
        processTime = calculateTime(distribution_name,temp);
    }
    t_next = t + processTime;
    d_Splitting_Component[splitting_name] -= 1;
}
//拆分机器从Uploading到Idle这个变迁上执行的动作
void Custom_Splitting_Uploading_Idle_Action(map<int,map<string,int>> Splitting_n,map<string,string> Material_Product_Name,string& Material_Assignment_Name,string Splitting_Flag,map<string,int> Splitting_Component,int Splitting_i,map<string,int> d_Splitting_Component){
    vector<string> flag;
	Custom_trim(Splitting_Flag);   //去除字符串中的空格
    //1.对Splitting_Flag进行拆分 
    //1.1判断Splitting_Flag是否可以进行拆分
    if(Splitting_Flag.find("or") != string :: npos){
        flag = Custom_get_split(Splitting_Flag,"or"); 
    }else
        flag.push_back(Splitting_Flag); 
    //2.由Machine_i得到满足条件的判断语句
    string satisfy_flag = flag[Splitting_i];
    //2.1 得到拆分零件的名称
    string splitting_name = Custom_get_split(satisfy_flag,">")[0];
    //2.2 得到拆分后的产品名称"集合"
    string all_product_name = Material_Product_Name[splitting_name];
    vector<string> product_name_list = Custom_get_split(all_product_name,"and");
    for(int i=0;i<product_name_list.size();i++){
        string name = product_name_list[i];
        int num = Splitting_n[Splitting_i][name];
        d_Splitting_Component[name] += num;
    }

    //重置
    Splitting_i = -1;
    Material_Assignment_Name = "";

}

/*
机械臂
*/
//判断机械臂是否满足从Idle到Busy的条件，若满足则返回true，否则返回false
bool Custom_get_RoboticArm_bool(string RoboticArm_Flag, map<string,int> RoboticArm_Component,int& RoboticArm_i){
    bool res = false;
    vector<string> flag;
	Custom_trim(RoboticArm_Flag);   //去除字符串中的空格
    if(RoboticArm_Flag.find("||") != string :: npos){
        flag = Custom_get_split(RoboticArm_Flag,"||"); 
    }else
        flag.push_back(RoboticArm_Flag); 

    for(int i = 0; i < flag.size(); i++){
        vector<string> part_flag = Custom_get_split(flag[i],"and");  
        if(Custom_judge(part_flag[0],RoboticArm_Component) and Custom_judge(part_flag[2],RoboticArm_Component))
        {   
            vector<string> temp = Custom_get_split(part_flag[1],"or");
            for(int j=0;j<temp.size();j++){
                if(Custom_judge(temp[j],RoboticArm_Component)){
                    res = true;
                    RoboticArm_i = i;
                    break;
                }
            }
			//string t = part_flag[0];
            if(res)
                break;
        }
    }


    return res;
}

//机械臂从Loading到Transporting这个变迁上执行的动作
void Custom_RoboticArm_Loading_Transporting_Action(string& Material_Assignment_Name,int& RoboticArm_Fetchnum,string RoboticArm_Id,string RoboticArm_Flag, map<string,int> RoboticArm_Component,int RoboticArm_i,map<string,int>& RoboticArm_Hold,map<string,int>& d_RoboticArm_Component){
    //1.对RoboticArm_Flag进行拆分
    vector<string> flag;
	Custom_trim(RoboticArm_Flag);   //去除字符串中的空格
    if(RoboticArm_Flag.find("||") != string :: npos){
        flag = Custom_get_split(RoboticArm_Flag,"||"); 
    }else
        flag.push_back(RoboticArm_Flag); 
    //2.由RoboticArm_i得到满足条件的判断语句
    string satisfy_flag = flag[RoboticArm_i];

    //3.得到机械臂此次可以抓取的数量min1
    vector<string> partflag = Custom_get_split(satisfy_flag,"and");
    int min1 = min(Custom_get_value(partflag[0],RoboticArm_Component),Custom_get_value(partflag[2],RoboticArm_Component));
    //4.得到初始化的机械臂持有的物料id及数量，初始化数量为0
    RoboticArm_Hold = Custom_get_hold(partflag[1]);
    //5.遍历RoboticArm_Hold并更新

    map<string, int>::iterator  iter;
    for(iter = RoboticArm_Hold.begin(); iter != RoboticArm_Hold.end(); iter++)
    {
        int value = min(RoboticArm_Component[iter->first],min1);
        RoboticArm_Hold[iter->first] = value; //iter->first:"Warehouse1_1_0_Num"

        //得到左侧、右侧实体名 方法一
        string left_entities_name = Custom_get_split(iter->first,"_")[0];
        string right_entities_name = Custom_get_split(Custom_get_split(partflag[2],">")[0],"_")[0];

        d_RoboticArm_Component[left_entities_name+"_Rest"] += value; //左侧实体托盘剩余容量+
        d_RoboticArm_Component[iter->first] -= value;  //左侧实体hold-
        d_RoboticArm_Component[RoboticArm_Id+"_Rest"] -= value; //机械臂托盘剩余容量-
        RoboticArm_Fetchnum += value;
        min1 = min((Custom_get_value(partflag[0],RoboticArm_Component) - RoboticArm_Fetchnum), (Custom_get_value(partflag[2],RoboticArm_Component)-RoboticArm_Fetchnum));
        Material_Assignment_Name+="Assignment" + Custom_get_split(iter->first,"_")[1] + " ";
        if(min1 == 0)break;
    }

}

//机械臂从Uploading到Idle这个变迁上执行的动作
void Custom_RoboticArm_Uploading_Idle_Action(string& Material_Assignment_Name,string RoboticArm_Flag,int& RoboticArm_i,int& RoboticArm_Fetchnum,map<string,int>& RoboticArm_Hold,string RoboticArm_Id,map<string,int>& d_RoboticArm_Component){
    //1.对RoboticArm_Flag进行拆分
    vector<string> flag;
	Custom_trim(RoboticArm_Flag);   //去除字符串中的空格
    if(RoboticArm_Flag.find("||") != string :: npos){
        flag = Custom_get_split(RoboticArm_Flag,"||"); 
    }else
        flag.push_back(RoboticArm_Flag); 
    //2.由RoboticArm_i得到满足条件的判断语句
    string satisfy_flag = flag[RoboticArm_i];

    vector<string> partflag = Custom_get_split(satisfy_flag,"and");

    map<string, int>::iterator  iter;
    for(iter = RoboticArm_Hold.begin(); iter != RoboticArm_Hold.end(); iter++)
    {
        
        //得到左侧、右侧实体名 方法一
        string left_entities_name = Custom_get_split(iter->first,"_")[0];
        string right_entities_name = Custom_get_split(Custom_get_split(partflag[2],">")[0],"_")[0];

        string temp = iter->first;
        d_RoboticArm_Component[right_entities_name+"_Rest"] -= RoboticArm_Hold[iter->first]; //右侧实体托盘剩余容量-
        d_RoboticArm_Component[right_entities_name+temp.erase(0,left_entities_name.size())] += RoboticArm_Hold[iter->first];  //右侧实体hold+
        d_RoboticArm_Component[RoboticArm_Id+"_Rest"] += RoboticArm_Hold[iter->first]; //机械臂托盘剩余容量+
    }

    //重置
    RoboticArm_i = -1;
    RoboticArm_Hold.erase(RoboticArm_Hold.begin(),RoboticArm_Hold.end());
    RoboticArm_Fetchnum = 0;
    Material_Assignment_Name = "";
}


/*
AGV
*/
//判断AGV是否满足从Idle到Prepareing的条件，若满足则返回true，否则返回false
bool Custom_get_AGV_bool(bool& AGV_waitFlag,int AGV_Rest,int AGV_Fetchnum,map<string, int> AGV_Hold, string AGV_Flag, map<string,int> AGV_Component,int& AGV_i){
	
    bool res = false;
    vector<string> flag;
	Custom_trim(AGV_Flag);   //去除字符串中的空格
    if(AGV_Flag.find("||") != string :: npos){
        flag = Custom_get_split(AGV_Flag,"||"); 
    }else
        flag.push_back(AGV_Flag); 

    for(int i = 0; i < flag.size(); i++){
        vector<string> part_flag = Custom_get_split(flag[i],"and");  
        if(Custom_judge(part_flag[0],AGV_Component) and Custom_judge(part_flag[2],AGV_Component))
        {   
            vector<string> temp = Custom_get_split(part_flag[1],"or");
            for(int j=0;j<temp.size();j++){
                if(Custom_judge(temp[j],AGV_Component)){
                    res = true;
                    AGV_i = i;
					//AGV节拍对应代码
					//得到AGV此次可以抓取的数量min1
					vector<string> partflag = Custom_get_split(flag[AGV_i], "and");
					 int min1 = min(Custom_get_value(partflag[0], AGV_Component), Custom_get_value(partflag[2], AGV_Component));
					//4.得到初始化的机械臂持有的物料id及数量，初始化数量为0
					AGV_Hold = Custom_get_hold(partflag[1]);
					map<string, int>::iterator  iter;
					for (iter = AGV_Hold.begin(); iter != AGV_Hold.end(); iter++)
					{
						int value = min(AGV_Component[iter->first], min1);
						if (value == 0)continue;
						AGV_Hold[iter->first] = value; //iter->first:"Warehouse1_1_0_Num"

						AGV_Fetchnum += value;
						min1 = min((Custom_get_value(partflag[0], AGV_Component) - AGV_Fetchnum), (Custom_get_value(partflag[2], AGV_Component) - AGV_Fetchnum));
						if (min1 == 0)break;
					}
					if (AGV_Fetchnum < AGV_Rest) {
						//if (AGV_waitCount == 0) {
							AGV_waitFlag = true;
						//}
					}
                    break;
                }
            }
            if(res)
                break;
        }
    }
    return res;
}

//AGV从Loading到Transporting这个变迁上执行的动作
void Custom_AGV_Loading_Transporting_Action(string& Material_Assignment_Name,int& AGV_Fetchnum,string AGV_Id,string AGV_Flag, map<string,int> AGV_Component,int AGV_i,map<string,int>& AGV_Hold,map<string,int>& d_AGV_Component){
    //1.对AGV_Flag进行拆分
    vector<string> flag;
	Custom_trim(AGV_Flag);   //去除字符串中的空格
    if(AGV_Flag.find("||") != string :: npos){
        flag = Custom_get_split(AGV_Flag,"||"); 
    }else
        flag.push_back(AGV_Flag); 
    //2.由AGV_i得到满足条件的判断语句
    string satisfy_flag = flag[AGV_i];
    //3.得到AGV此次可以抓取的数量min1
    vector<string> partflag = Custom_get_split(satisfy_flag,"and");
	//int min1 = min1 = min(Custom_get_value(partflag[0], AGV_Component), Custom_get_value(partflag[2], AGV_Component));

    int min1 = min(Custom_get_value(partflag[0],AGV_Component),Custom_get_value(partflag[2],AGV_Component));
    //4.得到初始化的AGV持有的物料id及数量，初始化数量为0
    AGV_Hold = Custom_get_hold(partflag[1]);
    //5.遍历RoboticArm_Hold并更新

    map<string, int>::iterator  iter;
    for(iter = AGV_Hold.begin(); iter != AGV_Hold.end(); iter++)
    {
        int value = min(AGV_Component[iter->first],min1);
		if (value == 0)continue;
        AGV_Hold[iter->first] = value; //iter->first:"Warehouse1_1_0_Num"
        //得到左侧、右侧实体名 方法一
        string left_entities_name = Custom_get_split(iter->first,"_")[0];
        string right_entities_name = Custom_get_split(Custom_get_split(partflag[2],">")[0],"_")[0];

        d_AGV_Component[left_entities_name+"_Rest"] += value; //左侧实体托盘剩余容量+
        d_AGV_Component[iter->first] -= value;  //左侧实体hold-
        d_AGV_Component[AGV_Id+"_Rest"] -= value; //AGV托盘剩余容量-
        AGV_Fetchnum += value;

        min1 = min((Custom_get_value(partflag[0],AGV_Component) - AGV_Fetchnum), (Custom_get_value(partflag[2],AGV_Component) - AGV_Fetchnum));
        Material_Assignment_Name+="Assignment" + Custom_get_split(iter->first,"_")[1] + " ";
        if(min1 == 0)break;
    }

}

//AGV从Uploading到Idle这个变迁上执行的动作
void Custom_AGV_Uploading_Idle_Action(map<string, int> AGV_Component, string& AGV_CurrentPosition,string& Material_Assignment_Name,string AGV_Flag,int& AGV_i,int& AGV_Fetchnum,map<string,int>& AGV_Hold,string AGV_Id,map<string,int>& d_AGV_Component){
    //1.对AGV_Flag进行拆分
    vector<string> flag;
	Custom_trim(AGV_Flag);   //去除字符串中的空格
    if(AGV_Flag.find("||") != string :: npos){
        flag = Custom_get_split(AGV_Flag,"||"); 
    }else
        flag.push_back(AGV_Flag); 
    //2.由AGV_i得到满足条件的判断语句
    string satisfy_flag = flag[AGV_i];

    vector<string> partflag = Custom_get_split(satisfy_flag,"and");

    map<string, int>::iterator  iter;
    for(iter = AGV_Hold.begin(); iter != AGV_Hold.end(); iter++)
    {
        //得到左侧、右侧实体名 方法一
        string left_entities_name = Custom_get_split(iter->first,"_")[0];
        string right_entities_name = Custom_get_split(Custom_get_split(partflag[2],">")[0],"_")[0];

        string temp = iter->first;
        d_AGV_Component[right_entities_name+"_Rest"] -= AGV_Hold[iter->first]; //右侧实体托盘剩余容量-
        d_AGV_Component[right_entities_name+temp.erase(0,left_entities_name.size())] += AGV_Hold[iter->first];  //右侧实体hold+
        d_AGV_Component[AGV_Id+"_Rest"] += AGV_Hold[iter->first]; //AGV托盘剩余容量+

        AGV_CurrentPosition = right_entities_name;
    }

    //重置
    AGV_i = -1;
    AGV_Hold.erase(AGV_Hold.begin(),AGV_Hold.end());
    AGV_Fetchnum = 0;
    Material_Assignment_Name = "";
}


/*
A连接
*/
//判断A是否满足从Idle到Idle的条件，若满足则返回true，否则返回false
bool Custom_get_A_bool(string A_Flag, map<string,int> A_Component,int& A_i){

    bool res = false;
    vector<string> flag;
	Custom_trim(A_Flag);   //去除字符串中的空格
    if(A_Flag.find("or") != string :: npos){
        flag = Custom_get_split(A_Flag,"or"); 
    }
    flag.push_back(A_Flag); 

    for(int i = 0; i < flag.size(); i++){
        vector<string> part_flag = Custom_get_split(flag[i],"and");  
        if(Custom_judge(part_flag[0],A_Component) and Custom_judge(part_flag[1],A_Component))
        {   
            res = true;
            A_i = i;
            break;
        }
    }
    return res;
}

//A连接从Idle->Idle这个变迁上执行的动作
void Custom_A_Idle_Idle_Action(string& Material_Assignment_Name,string A_Flag,map<string,int> A_Component,int& A_i,map<string,int>& d_A_Component){
    vector<string> flag;
	Custom_trim(A_Flag);   //去除字符串中的空格
    if(A_Flag.find("or") != string :: npos){
        flag = Custom_get_split(A_Flag,"or"); 
    }
    flag.push_back(A_Flag); 

    string satisfyflag = flag[A_i];
    vector<string> part_flag = Custom_get_split(satisfyflag,"and");
    string left_entities_name = get_entities_name(part_flag[0]);//得到左侧实体名称
    string right_entities_name = get_entities_name(part_flag[1]);//得到右侧实体名称

    // vector<string> entities_name = Custom_get_split(A_i_StartPosition_EndPosition[A_i],"_");
    // string left_entities_name = entities_name[0];//得到左侧实体名称
    // string right_entities_name = entities_name[1];//得到右侧实体名称

    Material_Assignment_Name = "Assginment"+Custom_get_split(Custom_get_split(part_flag[0],">")[0],"_")[1];

    string temp = Custom_get_split(part_flag[0],">")[0];  //比如:Warehouse2_2_2_Num
    
    d_A_Component[left_entities_name+"_Rest"] += 1;
    d_A_Component[temp] -= 1;
    d_A_Component[right_entities_name+"_Rest"] -= 1;
    d_A_Component[right_entities_name+temp.erase(0,left_entities_name.size())] += 1;
    
    //重置
    A_i = -1;
}



/*
All
*/
//All接收信号后的处理动作函数
void Custom_All_Receive_Signal_Action(string id, map<string, int>& Warehouse_Component,map<string, map<string,int>>& Combiner_Component,map<string,int> d_Combiner_Component,map<string, map<string,int>>& Splitting_Component,map<string,int> d_Splitting_Component,map<string, map<string,int>>& Machine_Component,map<string,int> d_Machine_Component,map<string, map<string,int>>& RoboticArm_Component,map<string,int> d_RoboticArm_Component,map<string, map<string,int>>& AGV_Component,map<string,int> d_AGV_Component,map<string,int>& A_Component,map<string,int> d_A_Component,map<string,int> d_Assignment_Component){
    if(id.find("Machine") != string::npos){
	    Machine_Component[id] = Custom_update_Component(d_Machine_Component,Machine_Component[id]);
        //遍历d_Machine_Component
        map<string,int>::iterator iter;
        for(iter = d_Machine_Component.begin(); iter != d_Machine_Component.end(); iter++)
        {
            if(d_Machine_Component[iter->first]!=0){
                string key_name = iter->first;
                //查找RoboticArm_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,RoboticArm_Component,iter->second);
                //查找AGV_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,AGV_Component,iter->second);
				//查找Warehouse_Component中是否存了该值，若有则更新
				check_string_in_map_and_upadte(key_name, Warehouse_Component, iter->second);
                //查找A_Component中是否存了该值,若有则更新
                check_string_in_map_and_upadte(key_name,A_Component,iter->second);
            }
        }
    }
	else if(id.find("Combiner") != string::npos){
	    Combiner_Component[id] = Custom_update_Component(d_Combiner_Component,Combiner_Component[id]);
        //遍历d_Combiner_Component
        map<string,int>::iterator iter;
        for(iter = d_Combiner_Component.begin(); iter != d_Combiner_Component.end(); iter++)
        {
            if(d_Combiner_Component[iter->first]!=0){
                string key_name = iter->first;
                //查找RoboticArm_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,RoboticArm_Component,iter->second);
                //查找AGV_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,AGV_Component,iter->second);
				//查找Warehouse_Component中是否存了该值，若有则更新
				check_string_in_map_and_upadte(key_name, Warehouse_Component, iter->second);
                //查找A_Component中是否存了该值,若有则更新
                check_string_in_map_and_upadte(key_name,A_Component,iter->second);
            }
        }
    }
    else if(id.find("Splitting") != string::npos){
	    Splitting_Component[id] = Custom_update_Component(d_Splitting_Component,Splitting_Component[id]);
        //遍历d_Combiner_Component
        map<string,int>::iterator iter;
        for(iter = d_Splitting_Component.begin(); iter != d_Splitting_Component.end(); iter++)
        {
            if(d_Splitting_Component[iter->first]!=0){
                string key_name = iter->first;
                //查找RoboticArm_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,RoboticArm_Component,iter->second);
                //查找AGV_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,AGV_Component,iter->second);
				//查找Warehouse_Component中是否存了该值，若有则更新
				check_string_in_map_and_upadte(key_name, Warehouse_Component, iter->second);
                //查找A_Component中是否存了该值,若有则更新
                check_string_in_map_and_upadte(key_name,A_Component,iter->second);
            }
        }
    }
    else if(id.find("RoboticArm") != string::npos){
        RoboticArm_Component[id] = Custom_update_Component(d_RoboticArm_Component,RoboticArm_Component[id]);
        //遍历d_RoboticArm_Component
        map<string,int>::iterator iter;
        for(iter = d_RoboticArm_Component.begin(); iter != d_RoboticArm_Component.end(); iter++)
        {
            if(d_RoboticArm_Component[iter->first]!=0){
                string key_name = iter->first;
                //查找Machine_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,Machine_Component,iter->second);
                //查找Combiner_Component中是否存了该值，若有则更新
                check_string_in_doublemap_and_upadte(key_name,Combiner_Component,iter->second);
                //查找Splitting_Component中是否存了该值，若有则更新
                check_string_in_doublemap_and_upadte(key_name,Splitting_Component,iter->second);
                //查找AGV_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,AGV_Component,iter->second);
				//查找Warehouse_Component中是否存了该值，若有则更新
				check_string_in_map_and_upadte(key_name, Warehouse_Component, iter->second);
                //查找A_Component中是否存了该值,若有则更新
                check_string_in_map_and_upadte(key_name,A_Component,iter->second);
            }
        }
    }
    else if(id.find("AGV") != string::npos){
		//由d_AGV_Component更新AGV_Component
        AGV_Component[id] = Custom_update_Component(d_AGV_Component,AGV_Component[id]);
        //遍历d_AGV_Component
        map<string,int>::iterator iter;
        for(iter = d_AGV_Component.begin(); iter != d_AGV_Component.end(); iter++)
        {
            if(d_AGV_Component[iter->first] != 0){
                string key_name = iter->first;
                //查找RoboticArm_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,RoboticArm_Component,iter->second);
                //查找Machine_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,Machine_Component,iter->second);
                //查找Combiner_Component中是否存了该值，若有则更新
                check_string_in_doublemap_and_upadte(key_name,Combiner_Component,iter->second);
                //查找Splitting_Component中是否存了该值，若有则更新
                check_string_in_doublemap_and_upadte(key_name,Splitting_Component,iter->second);
				//查找Warehouse_Component中是否存了该值，若有则更新
				check_string_in_map_and_upadte(key_name, Warehouse_Component, iter->second);
                //查找A_Component中是否存了该值,若有则更新
                check_string_in_map_and_upadte(key_name,A_Component,iter->second);
            }
        }
    }
	else if(id.find("Assignment") != string::npos){
            
            //遍历d_Assignment_Component
            map<string,int>::iterator iter;
            for(iter = d_Assignment_Component.begin(); iter != d_Assignment_Component.end(); iter++)
            {
                string key_name = iter->first;
				key_name = key_name +"_"+ id.substr(id.size()-1) + "_0_Num";
                //查找AGV_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,AGV_Component,iter->second);
                //查找RoboticArm_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,RoboticArm_Component,iter->second);
                //查找Machine_Component中是否存了该值,若有则更新
                check_string_in_doublemap_and_upadte(key_name,Machine_Component,iter->second);
                //查找Combiner_Component中是否存了该值，若有则更新
                check_string_in_doublemap_and_upadte(key_name,Combiner_Component,iter->second);
                //查找Splitting_Component中是否存了该值，若有则更新
                check_string_in_doublemap_and_upadte(key_name,Splitting_Component,iter->second);
                //查找A_Component中是否存了该值,若有则更新
                check_string_in_map_and_upadte(key_name,A_Component,iter->second);
				//查找Warehouse_Component中是否存了该值，若有则更新，若无，则添加
				check_string_in_map_and_upadte(key_name, Warehouse_Component,iter->second);
            }
    }
    else if(id.find("A") != string::npos){
            A_Component = Custom_update_Component(d_A_Component,A_Component);
            //遍历d_A_Component
            map<string,int>::iterator iter;
            for(iter = d_A_Component.begin(); iter != d_A_Component.end(); iter++)
            {
                if(d_A_Component[iter->first] != 0){
                    string key_name = iter->first;
                    //查找AGV_Component中是否存了该值,若有则更新
                    check_string_in_doublemap_and_upadte(key_name,AGV_Component,iter->second);
                    //查找RoboticArm_Component中是否存了该值,若有则更新
                    check_string_in_doublemap_and_upadte(key_name,RoboticArm_Component,iter->second);
                    //查找Machine_Component中是否存了该值,若有则更新
                    check_string_in_doublemap_and_upadte(key_name,Machine_Component,iter->second);
                    //查找Combiner_Component中是否存了该值，若有则更新
                    check_string_in_doublemap_and_upadte(key_name,Combiner_Component,iter->second);
                    //查找Splitting_Component中是否存了该值，若有则更新
                    check_string_in_doublemap_and_upadte(key_name,Splitting_Component,iter->second);
					//查找Warehouse_Component中是否存了该值，若有则更新
					check_string_in_map_and_upadte(key_name, Warehouse_Component, iter->second);
                }
            }
        }
}

/*
随机分布
*/
/*随机分布函数实现
Random_Distribution_Name:随机分布名称
Random_Distribution_Value:存储该随机分布用到的变量名及具体数值
*/
double calculateTime(string Random_Distribution_Name,map<string,map<string,double>> Random_Distribution_Value){

    double res = 0.0;
    
    map<string,double> temp = Random_Distribution_Value[Random_Distribution_Name];

    int tm=(int)time(NULL);
    srand(tm);

    if(Random_Distribution_Name == "binomial"){
        double N = temp["N"];
        double p = temp["p"];
        res = binomial(N,p);
    }
    if(Random_Distribution_Name == "Possion"){
        double lamda = temp["lamda"];
        res = Possion(lamda);
    }
    if(Random_Distribution_Name == "Exponential"){
        double lamda = temp["lamda"];
        res = Exponential(lamda);
    }
    if(Random_Distribution_Name == "gaussrand"){
        double mean = temp["mean"];
        double stdc = temp["stdc"];
        res = gaussrand(mean,stdc);
    }
    if(Random_Distribution_Name == "triDistribution"){
        double x = temp["x"];
        double minValue = temp["minValue"];
        double maxValue = temp["maxValue"];
        res = triDistribution(x,minValue,maxValue);
    }

    return res;

}

//二项分布
double binomial(double	N,double  p)
{
    double rnd = 0;
	for (long i=0;i<N;i++)
	{
		double pV = (double)rand()/(double)RAND_MAX;
		if (pV<p)
		{
			rnd++;
		}
	}
	return setPrecision(rnd,1);
    
}
 
//泊松分布
double Possion(double lambda)
{
    double x = -1, u;  
    double log1, log2;
    log1 = 0;
    log2 = -lambda;
    do
    {
        u = (double)rand()/(double)RAND_MAX;   
        log1 += log(u);
        x++;   
    }while(log1 >= log2);
    return setPrecision(x,1);   
}


//指数分布
double Exponential(double lamda)
{
    double pV = 0.0;
    while(true)
    {
        pV = (double)rand()/(double)RAND_MAX;
        if (pV != 1)
        {
            break;
        }
    }
    pV = (-1.0/lamda)*log(1-pV);
    return setPrecision(pV,1);
}

double gaussrand_NORMAL() {
	static double V1, V2, S;
	static int phase = 0;
	double X;


	if (phase == 0) {
		do {
			double U1 = (double) rand() / RAND_MAX;
			double U2 = (double) rand() / RAND_MAX;


			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);


		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);


	phase = 1 - phase;


	return setPrecision(X,1);
}

//正态分布 mean均值， stdc 方差
double gaussrand(double mean, double stdc) {
	return setPrecision((mean + gaussrand_NORMAL() * stdc),1);
}

//三角分布 除x外的三个值不能为同一个值 x∈[minValue,mode] or x∈ (mode,maxValue]
double triDistribution(double x, double minValue, double MaxValue){
    double res = 0;
    int mode =  rand() & (int)minValue;
    if(x >= minValue && x <= mode){
        res = 2 * (x - minValue) /((MaxValue - minValue) * (mode - minValue));
    }else{
       res =  2 * (MaxValue - x) / ((MaxValue - minValue) * (MaxValue - mode));
    }
    return setPrecision(res,1);

}

};
#endif
