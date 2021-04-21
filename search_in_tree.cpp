#include "search_in_tree.h"
//主要用来将路径按'\\'分开。例：字符串"C:\\Users\\Administrator" 切片结果为"C:" "Users" "Administrator"
void split(string s, char ch, vector<string> &vst)//字符串切片函数，将s按ch切片。切片结果放进vst
{
    vst.clear();//记得清空
    istringstream iss(s);
    string temp;
    while(getline(iss, temp, ch)) vst.push_back(temp);
}
//监听到文件系统有删除文件时，响应删除文件。在树里删除文件对应节点。参数为被删除节点的绝对路径，包含文件名
void del(const vector<string> &vst)
{
    int p;
    //p为待删节点，q为待删节点的父节点的第一个子节点
    if(p=findNode(vst))//如果找到了需要删除的节点
    {
        int q=um[um[p].father].son;//得到该节点父节点的第一个子节点
        //若待删除节点是父节点的第一个子节点，则父节点的第一个子节点变成该节点的兄弟节点，即他的下一个节点
        if(p==q) um[um[p].father].son=um[p].bro;
        else
        {
            //如果不是，则找到带删节点的前一个节点，令前一个节点的兄弟节点为自己的兄弟节点。即跳过自己
            while(um[q].bro != p)
            {
                if(um[q].bro)//如果不为0，说明q有兄弟，则指向兄弟
                    q=um[q].bro;
                else//如果q已经没有兄弟了，说明 找到了待删节点的位置，但从其父节点的第一个子节点走过来却走不到
                    //显然在此时树是有问题的，不过应该不存在这种可能，即这个else应该不会被执行
                {
                    cout<<"delete fail!\nfinaly file is "<<um[q].name<<endl;
                    return;
                }
            }//end while
            um[q].bro=um[p].bro;
        }//end else
        um.erase(p);//删除此节点
    }
//    else//找不到待删节点的位置，直接忽略掉
//        cout<<"delete fail\n";
}
//更新节点，即文件名。参数为待更新节点的绝对路径，包含old name。和new name
void updata(const vector<string> &vst, string name)
{
    int p;
    if(p=findNode(vst))//如果找到了该节点，直接改名即可
    {
        um[p].name=name;
    }
//    else//找不到该节点，忽略掉
//        cout<<"update faile!\n";
}
//添加节点。参数为待新增节点的父路径，不包含文件名。以及待新增文件的文件名
void append(const vector<string> &vst, string name)
{
    int p;
    if(p=findNode(vst))//如果找到了新增文件的父节点位置
        //则新增一个节点，并插在父节点的第一个子节点。可以理解为头插法。
    {
        int &son=um[p].son;//没什么用
        endx++;//给新增节点分配位置
        um[endx]={name, getCheckType(name.c_str()), p, 0, son};//新增节点。由于是哈希表实现，新增的时间复杂度为O(1)
        um[p].son=endx;//
//        cout<<"endx="<<endx<<", p="<<p<<", name="<<um[um[p].son].name<<" "<<name<<endl;
    }
//    else
//    {
//        cout<<"file: "<<name<<" append fail!\n";
//    }
}
//字符串转小写
string tolow(string s)
{
    string res="";
    for(auto i:s)
    {
        if(i>='A' && i<='Z') res+=i+32;
        else res+=i;
    }
    return res;
}
//查找节点位置
int findNode(const vector<string> &vst)
{
    int p=1, i=0, len=vst.size();//p为当前节点位置，i为当前节点深度，len为待查节点深度
    if(!len) return -1;//深度为0，则全盘查找。由于自己作，本来应该返回0的，但0同时也是虚拟的所有盘的父节点。不得已改成返回-1
    while(p)
    {
//        if(i==len-1) cout<<"p="<<p<<", name="<<um[p].name<<", vst="<<vst[i]<<endl;

        if(tolow(um[p].name) == tolow(vst[i]))//不区分大小写，因为windows的文件名默认是不区分大小写的
        {
            i++;
            if(i==len) return p;//已经找到了，返回节点位置
            else p=um[p].son;//还没到目标位置，查找子树
        }
        else p=um[p].bro;//查找兄弟
    }
//    cout<<"sorry, not find the node\n";//没有找到位置，树出问题了，忽略掉
//    for(auto i:vst) cout<<i<<"\\";//输出一下路径
//    cout<<"\n";
    return 0;
}
//给定位置，返回其绝对路径。返回的绝对路径包含自己
string getPath(int x)
{
    string s="";//待返回的路径
//    x=um[x].father;//去掉这句注释，返回的路径就不会包含自己，但其他地方要改的代码会很多，所以就没要了
    while(x)//当x为0时，表示已经找到了根
    {
        if(s!="") s=um[x].name+"\\"+s;
        else s=um[x].name;
        x=um[x].father;//父节点位置
    }
    return s;
}

void cal_next(char *str, int *next, int len)
{
    next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
    int k = -1;//k初始化为-1
    for (int q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
        {
            k = next[k];//往前回溯
        }
        if (str[k + 1] == str[q])//如果相同，k++
        {
            k = k + 1;
        }
        next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
    }
}

int KMP(char *str, char *ptr)
{
    int slen=strlen(str), plen=strlen(ptr);
    int *next = new int[plen];
    cal_next(ptr, next, plen);//计算next数组
    int k = -1;
    for (int i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])//ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
            k = next[k];//往前回溯
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)//说明k移动到ptr的最末端
        {
            return i+1;//返回相应的位置
        }
    }
    return 0;
}

bool ok(string s, string p1, bool f, bool reg)
{
    if(!f)//不区分大小写
    {
        transform(s.begin(),s.end(),s.begin(),::tolower);//母串改小写
        transform(p1.begin(),p1.end(),p1.begin(),::tolower);//子串改小写
    }
    if(reg)//使用正则进行匹配
    {
        QRegExp re(QObject::trUtf8(p1.c_str()));
        return re.exactMatch(s.c_str());
    }
    int lp=p1.length();//子串长度
    char p[lp+1];
    memset(p,0,sizeof(p));
    for(int i=0; i<lp; i++) p[i]=((p1[i]=='*' || p1[i]=='?') ? '\0' : p1[i]);//0表示可为任意字符
    int pa=0;//匹配的串长度
    for(int i=0; i<lp;)//子串遍历
    {
        if(p1[i]=='*')//*可以匹配任意长字符
        {
            if(i==lp-1) return 1;//如果匹配完了，说明匹配成功
            int l=KMP(&s[pa],&p[++i]);//匹配的长度
            if(!l) return 0;//匹配长度为0，即完全不匹配。则后面不可能匹配成功
            pa+=l;//
            i+=strlen(&p[i]);
            continue;
        }
        if(p1[i]=='?') pa++, i++;//可以匹配任意单个字符
        while(p[i]!='\0')
        {
            if(p[i]==s[pa]) i++, pa++;//匹配则往后继续匹配
            else return 0;//不匹配，则返回不匹配
        }
    }
    return pa==s.length() ? 1 : 0;
}
