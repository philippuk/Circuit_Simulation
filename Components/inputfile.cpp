#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
//#include "Component.hpp"
//#include "Node.hpp"
//#include "Resistor.hpp"

using namespace std;

int main()
{
   
    string s;
    ifstream infile("Test1.1.txt");   

    vector<int> Rnode, Rnode2;
    vector<double> vr;

    vector<int> Vnode, Vnode2;
    vector<double> vv;

    vector<int> Inode, Inode2;
    vector<double> vi;

    vector<int> Cnode, Cnode2;
    vector<double> vc;

    vector<int> Lnode, Lnode2;
    vector<double> vl;





    while (getline(infile, s)){
       if(s[0]=='R'){
           
           size_t rpos = s.find('N');
           int rnodepos = stoi(s.substr(rpos+1,3));               
           Rnode.push_back(rnodepos);
           
           
           int rnodepos2;
           size_t rpos2 = rpos+5;
           
           if(s[rpos2]=='0'){

               rnodepos2 = 0;
               Rnode2.push_back(rnodepos2);
               double rnum = stod(s.substr(rpos2+2));
               
               if(s[s.size()-1]=='k'){

                   vr.push_back(rnum*pow(10,3));

               }else if(s[s.size()-1]=='M'){

                   vr.push_back(rnum*pow(10,6));

               }else if(s[s.size()-1]=='G'){

                   vr.push_back(rnum*pow(10,9));

               };
               

           }else if(s[rpos2]=='N'){
               
               rnodepos2 = stoi(s.substr(rpos2+1,3));
               Rnode2.push_back(rnodepos2);
               
               double rnum = stod(s.substr(rpos2+5));
               
               if(s[s.size()-1]=='k'){

                   vr.push_back(rnum*pow(10,3));

               }else if(s[s.size()-1]=='M'){

                   vr.push_back(rnum*pow(10,6));

               }else if(s[s.size()-1]=='G'){

                   vr.push_back(rnum*pow(10,9));

               };

           }  
           
              
       }else if(s[0]=='V'){

           size_t vpos = s.find('N');
           int vnodepos = stoi(s.substr(vpos+1,3));               
           Vnode.push_back(vnodepos);
           
           
           int vnodepos2;
           size_t vpos2 = vpos+5;
           
           if(s[vpos2]=='0'){

               vnodepos2 = 0;
               Vnode2.push_back(vnodepos2);
               vv.push_back(stod(s.substr(vpos2+2)));

           }else if(s[vpos2]=='N'){
               
               vnodepos2 = stoi(s.substr(vpos2+1,3));
               Vnode2.push_back(vnodepos2);
               vv.push_back(stod(s.substr(vpos2+5)));

           }  




       }else if(s[0]=='I'){

           size_t ipos = s.find('N');
           int inodepos = stoi(s.substr(ipos+1,3));               
           Inode.push_back(inodepos);
           
           
           int inodepos2;
           size_t ipos2 = ipos+5;
           
           if(s[ipos2]=='0'){

               inodepos2 = 0;
               Inode2.push_back(inodepos2);
               vi.push_back(stod(s.substr(ipos2+2)));

           }else if(s[ipos2]=='N'){
               
               inodepos2 = stoi(s.substr(ipos2+1,3));
               Inode2.push_back(inodepos2);
               vi.push_back(stod(s.substr(ipos2+5)));

           }  

       }else if(s[0]=='C'){

           size_t cpos = s.find('N');
           int cnodepos = stoi(s.substr(cpos+1,3));               
           Cnode.push_back(cnodepos);
           
           
           int cnodepos2;
           size_t cpos2 = cpos+5;
           
           if(s[cpos2]=='0'){

               cnodepos2 = 0;
               Cnode2.push_back(cnodepos2);
               
               double cnum = stod(s.substr(cpos2+2));
               
               if(s[s.size()-1]=='p'){

                   vc.push_back(cnum*pow(10,-12));

               }else if(s[s.size()-1]=='n'){

                   vc.push_back(cnum*pow(10,-9));

               }else if(s[s.size()-1]=='u'){

                   vc.push_back(cnum*pow(10,-6));

               }else if(s[s.size()-1]=='m'){

                   vc.push_back(cnum*pow(10,-3));

               };

           }else if(s[cpos2]=='N'){
               
               cnodepos2 = stoi(s.substr(cpos2+1,3));
               Cnode2.push_back(cnodepos2);
               
               double cnum = stod(s.substr(cpos2+5));
               
               if(s[s.size()-1]=='p'){

                   vc.push_back(cnum*pow(10,-12));

               }else if(s[s.size()-1]=='n'){

                   vc.push_back(cnum*pow(10,-9));

               }else if(s[s.size()-1]=='u'){

                   vc.push_back(cnum*pow(10,-6));

               }else if(s[s.size()-1]=='m'){

                   vc.push_back(cnum*pow(10,-3));

               }

           };  
       }else if(s[0]=='L'){

           size_t lpos = s.find('N');
           int lnodepos = stoi(s.substr(lpos+1,3));               
           Lnode.push_back(lnodepos);
           
           
           int lnodepos2;
           size_t lpos2 = lpos+5;
           
           if(s[lpos2]=='0'){

               lnodepos2 = 0;
               Lnode2.push_back(lnodepos2);
               
               double lnum = stod(s.substr(lpos2+2));

               if(s[s.size()-1]=='p'){

                   vl.push_back(lnum*pow(10,-12));

               }else if(s[s.size()-1]=='n'){

                   vl.push_back(lnum*pow(10,-9));

               }else if(s[s.size()-1]=='u'){

                   vl.push_back(lnum*pow(10,-6));

               }else if(s[s.size()-1]=='m'){

                   vl.push_back(lnum*pow(10,-3));

               };

           

           }else if(s[lpos2]=='N'){
               
               lnodepos2 = stoi(s.substr(lpos2+1,3));
               Lnode2.push_back(lnodepos2);
               vl.push_back(stod(s.substr(lpos2+5)));
               double lnum = stod(s.substr(lpos2+5));

               if(s[s.size()-1]=='p'){

                   vl.push_back(lnum*pow(10,-12));

               }else if(s[s.size()-1]=='n'){

                   vl.push_back(lnum*pow(10,-9));

               }else if(s[s.size()-1]=='u'){

                   vl.push_back(lnum*pow(10,-6));

               }else if(s[s.size()-1]=='m'){

                   vl.push_back(lnum*pow(10,-3));

               }

           }  
       }

       
    }
    
    
    infile.close();
}

