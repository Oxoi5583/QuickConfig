# QuickConfig - a ez-to-use config library

## Introduction
This is a simple ez-to-use config library. I built it just for practicing and personal development.

## Usage
1. Create a config folder

2. Create .qc file in ./config (.qc stand for Quick Config). By the following format.

    [QuickConfig::<Config_Namespace>]
    <data_type1>@<config_key1>::<value1>
    <data_type2>@<config_key2>::<value2>
    <data_type3>@<config_key3>::<value3>

    i. Config_Namespace is the namespace for your config
    ii. data_type should be 'n' (Numberic), 's' (String), 'b' (Boolean)
    iii. config_key is the key for searching value
    iv. value should align to the data_type

Here is a valid example for .qc

    [QuickConfig::Test_Config]
    n@num_name::3.1415
    s@str_name::test
    b@bool_name::true

![image](https://github.com/user-attachments/assets/b99e6ca3-0033-4ce7-b049-f4d1429f6282)
![image](https://github.com/user-attachments/assets/34fe14a4-7440-408f-ad97-82da9033ca82)

Remark : .qc Config_Namespace can be duplicated
         but config_key should be unique in one namespace.
         And you can use // to comment in the .qc file

    
4. include the header file "QuickConfig.h" & link the lib file

![image](https://github.com/user-attachments/assets/455fb5da-ee0b-4297-a6e7-926fe38abbcf)

5. In your own source, Create a QC_Path Object. And Move to ./config

![image](https://github.com/user-attachments/assets/6e74e724-3d2c-4963-a328-48ebb0fa317b)

6. Register the QC_Path on QC_Server

![image](https://github.com/user-attachments/assets/86675599-64be-4d9e-8d5e-6bc4cd8172df)

7. Get value from QC_Server

![image](https://github.com/user-attachments/assets/ba7ffd5e-6dca-4952-80c2-16569c9016bc)
