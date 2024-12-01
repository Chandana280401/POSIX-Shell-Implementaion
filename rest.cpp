while (true){
            ch = getchar();
            if(ch == '\n') {  
                break;
            } else if (ch == 127){  //backspace
                if (!line.empty()) {
                    line.pop_back();   
                }
                cout << "\033[2K\r";
                cout<<"\033[32m"<<str<<"\033[0m";
                cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
                cout<<line;     
            }else if(ch == '\t'){
                if(line!=""){
                    int pos=line.find(' ');            
                    if(pos != string::npos){
                        string s=filesuggestionfun(line);
                        tab=1;
                        if(s==""){
                            
                            break;
                        }
                        pos = line.find_last_of(' ');
                        line = line.substr(0,pos + 1) + s;
                        
                        cout << "\033[2K\r";
                        cout<<"\033[32m"<<str<<"\033[0m";
                        cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
                        cout<<line;
                    }else{
                        string s=commandsuggestionfun(line);
                        if(s!=""){
                            line="";
                            break;  
                        }else{
                            line="";
                            break;
                        }                           
                    }
                }
            }else if(ch == '\033'){ 
                char seq[2];
                seq[0] = getchar();
                seq[1] = getchar();
                if (seq[0] == '[') {
                    if (seq[1] == 'A') {
                        if(k>0){
                            k--;
                        }
                        cout << "\033[2K\r";
                        cout<<"\033[32m"<<str<<"\033[0m";
                        cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
                        line=history[k];
                        cout<<line;
                    }else if(seq[1]=='B'){
                        if(k<history.size()){
                            k++;
                        }
                    cout << "\033[2K\r";
                    cout<<"\033[32m"<<str<<"\033[0m";
                    cout<<"\033[34m"<<currpath<<"\033[0m"<<">";
                    line=history[k];
                    cout<<line;                                      
                 }
                }
            }else if(ch == '\x1A'){  //  Ctrl+Z
                    signal(SIGTSTP, SIG_DFL); 
                    raise(SIGTSTP); 
            } else if (ch == '\x03'){  //  Ctrl+C
                    if(cin.eof()==1){
                        exit(0);
                    }   
            } else if (ch == '\x04'){  //  Ctrl+D
                    int pid=getpid();
                    cout << "Interrupted";
                    kill(pid,SIGTSTP);
                    exit(0);
            }else {
                line += ch; 
                cout << ch;
            }
        
        }