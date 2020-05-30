#ifndef Component_hpp
#define Component_hpp

class Component
{
protected:
    double com_time=0;
    double com_timestep;
public:
    void set_time(double time){
        com_time=time;
    }
    
    void set_timestep(double timestep){
        com_timestep=timestep;
    }
    
    void change_time(bool sign){
        if (sign){
            com_time += com_timestep;
        }else{
            com_time -= com_timestep;
        }
    }
    
    double time(){
        return com_time;
    }
    
    double timestep(){
        return com_timestep;
    }
}

#endif /* Component_hpp */
