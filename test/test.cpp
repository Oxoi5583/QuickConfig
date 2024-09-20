#include <iostream>
#include <QuickConfig.h>

class Test{
    public: 
        std::string s = "";
};

int main(){
    std::cout << "START" << std::endl;


    QuickConfig::QC_Path _path = QuickConfig::QC_Path();
    _path.go_to_child("config");
    QC_Server->add_config_path(_path);

    std::string _s_ptr;

    Test t;

    QC_Server->get("Test_Config","str_name3",t.s);
    std::cout << "_s_ptr value : " << t.s << "" << std::endl;
    QC_Server->get("Test_Config","str_name2",t.s);
    std::cout << "_s_ptr value : " << t.s << "" << std::endl;
    QC_Server->get("Test_Config","str_name3",t.s);
    std::cout << "_s_ptr value : " << t.s << "" << std::endl;
    QC_Server->get("Test_Config","str_name",t.s);
    std::cout << "_s_ptr value : " << t.s << "" << std::endl;
    std::cout << "END" << std::endl;
    return 0;
}

