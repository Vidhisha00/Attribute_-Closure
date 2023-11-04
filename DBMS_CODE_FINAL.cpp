#include<iostream>
#include<bits/stdc++.h>
#include<vector>
#include<set>
using namespace std;

set<char> attribute_closure(int num,string fd[],set<char>closure)
{
     for(int m=0;m<num;m++)
    { int t=0;
    while(t<num)
    {
        //FD:X->Y
        //separate lhs and rhs using character vector 
        //if all elements of lhs are present in closure set
        //then we will add all the attributes of rhs to the closure set
        vector<char>rhs;
        vector<char>lhs;
        int flag=0;
        
        for(int i=0;i<fd[t].length();i++)
        {
            if((fd[t][i]>='A' && fd[t][i]<='Z') || (fd[t][i]>='a' && fd[t][i]<='z'))
            {
                if(flag!=1)
                lhs.push_back(fd[t][i]);
                else
                rhs.push_back(fd[t][i]);
            }
            else
            flag=1;

        }
        int check=0;
        for(int j=0;j<lhs.size();j++)
        {
            auto it=closure.find(lhs[j]);
            if(it==closure.end())
            {
                check=1;
                break;
            }
        }
        if(check==0)
        {
            for(int i=0;i<rhs.size();i++)
            closure.insert(rhs[i]);
        }
        t++;
    }
    }
    return closure;
}
set<string> subsequence(string str)
{
    set<string> st;
 
    // Iterate over the entire string
    for (int i = 0; i < str.length(); i++) {
 
        // Iterate from the end of the string
        // to generate substrings
        for (int j = str.length(); j > i; j--) {
            string sub_str = str.substr(i, j);
            st.insert(sub_str);
 
            // Drop kth character in the substring
            // and if its not in the set then recur
            for (int k = 1; k < sub_str.length(); k++) {
                string sb = sub_str;
 
                // Drop character from the string
                sb.erase(sb.begin() + k);
                subsequence(sb);
            }
        }
    }
    return st;
}
int main()
{
    string relation;
    cout<<"enter attributes present in relation(eg:{A,B,C,D} enclose within curly brackets):";
    cin>>relation;
    cout<<"enter no of FD's present in relation:";
    int num;
    cin>>num;
    string fd[num];
    string attributes[num];
    vector<string>canonical;
    //canonical will contain all strings such that there is only one attribute on it's right side
    for(int i=0;i<num;i++)
    {
        cout<<"enter fd in form of:{X->Y}:";
        cin>>fd[i];
        string res="";
      
        int j=0;
            while(fd[i][j]!='-')
            {
                res+=fd[i][j];
                j++;
            }
            attributes[i]=res;
       

    }
    for(int i=0;i<num;i++)
    {
        string temp="";
        int j=0;
        while(fd[i][j]!='>')
        {
            temp+=fd[i][j];
            j++;
        }
        temp+=fd[i][j];
        j++;
        
        while(j<fd[i].length())
        {
            string add=temp+fd[i][j];
            canonical.push_back(add);
            j++;

        }

    }
    //for(auto it:canonical)
    //cout<<it<<endl;
  
    //ATTRIBUTES ARRAY OF STRINGS CONTAIN ALL THE ATTRIBUTES PRESENT IN LHS
    /*cout<<"enter attribute to find closure of:";
    string tofind;
    cin>>tofind;*/
    vector<pair<string,set<char>>>lhs_closure(num);
    for(int i=0;i<num;i++)
    {
        string tofind=attributes[i];
        set<char>closure;
        for(int i=0;i<tofind.length();i++)
        {
            closure.insert(tofind[i]);
        }
        set<char>ans;
        ans=attribute_closure(num,fd,closure);
        lhs_closure.push_back({tofind,ans});

    }
    //TO FIND CANONICAL COVER WE HAVE TO ELEMINATE FD'S ONE BY ONE AND FIND OUT IF IT'S CLOSURE REMAINS SAME OR NOT
    vector<string>minimal;
    for(int i=0;i<canonical.size();i++)
    {
        string fd_canonical[canonical.size()-1];
        set<char>n;
        string temp=canonical[i];
        string to_find="";
        int siz=0;
        while(temp[siz]!='-')
        {
        n.insert(temp[siz]);
        to_find+=temp[siz];
        siz++;
        }
        int m=0;
        for(int t=0;t<canonical.size();t++)
        {
            if(t!=i)
            {
                fd_canonical[m]=canonical[t];
                m++;
            }
        }
        //we have to find closure of attribute from this set and compare it with closure_lhs
        set<char>compare1;
        compare1=attribute_closure(canonical.size()-1,fd_canonical,n);
        set<char>compare2;
        compare2=attribute_closure(num,fd,n);
        if(compare1!=compare2)
        minimal.push_back(temp);

    }
    //FUNCTION TO REMOVE EXTRANEOUS ATTRIBUTES
    vector<string>minimal_cover;
    //creating an array of strings of minimal cover
    string test[minimal.size()];
    for(int i=0;i<minimal.size();i++)
    {
        test[i]=minimal[i];
    }
    for(int i=0;i<minimal.size();i++)
    {
        string temp=minimal[i];
        int j=0;
        string res="";
       
        while(temp[j]!='-')
        {
            res+=temp[j];
            j++;

        }
        set<char>entire;
        for(int i=0;i<res.size();i++)
        entire.insert(res[i]);
        if(j>1)
        {
            set<string>sub_sequence=subsequence(res);
            int flag=0;
            string final="";
            //set sub_sequence has all the subsequence of lhs side of string
            //now if any of the subsequence has the same closure as that of the entire string we will replace it with that
            for(auto it:sub_sequence)
            {
                string t=it;
                set<char>temp_closure;
                for(int i=0;i<t.size();i++)temp_closure.insert(t[i]);
                set<char>temp1=attribute_closure(minimal.size(),test,temp_closure);
                set<char>temp2=attribute_closure(minimal.size(),test,entire);
                if(temp1==temp2)
                {
                    string a=it;
                    string b="";
                    while(j!=temp.size())
                    {
                        b+=temp[j];
                        j++;
                    }
                    final=a+b;
                    flag=1;
                    break;
                }
            }
            if(flag==0)
            minimal_cover.push_back(minimal[i]);
            else
            minimal_cover.push_back(final);

        }
        else
        minimal_cover.push_back(minimal[i]);



    }

    cout<<"attribute closures are:"<<endl;
    for(auto it:lhs_closure)
    {
        if(it.second.size()>0)
        {
        cout<<it.first<<":";
        for(auto v:it.second)
        cout<<v<<" ";
        cout<<endl;
        }
    }

    cout<<"minimal cover of the relation is:"<<endl;
    for(auto v:minimal_cover)
    cout<<v<<endl;
   
 
    /*set<char>::iterator itr;
    for(itr=closure.begin();itr!=closure.end();itr++)
    cout<<*itr<<" ";
    */
   
    //while loop to iterate through all the fd's
   
   

    /*set<char>::iterator it;
    for(it=closure.begin();it!=closure.end();it++)
    cout<<*it<<" ";
    cout<<endl;*/
    




}