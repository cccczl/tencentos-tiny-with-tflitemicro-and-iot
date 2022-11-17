#!/usr/bin/python
# -*- coding: utf-8 -*-

import json
import sys
import os
import argparse
import glob
#import cStringIO

reload(sys)
sys.setdefaultencoding("utf-8")

try: import simplejson as json
except: import json


# {"version":"1.0","properties":[{"id":"light_switch","name":"电灯开关","desc":"控制电灯开灭","required":true,"mode":"rw","define":{"type":"bool","mapping":{"0":"关","1":"开"}}},{"id":"color","name":"颜色","desc":"灯光颜色","mode":"rw","define":{"type":"enum","mapping":{"0":"Red","1":"Green","2":"Blue"}}},{"id":"brightness","name":"颜色","desc":"灯光颜色","mode":"rw","define":{"type":"int","unit":"%","unitDesc":"亮度百分比","min":"0","max":"100"}},{"id":"name","name":"灯位置名称","desc":"灯位置名称：书房、客厅等","mode":"rw","required":true,"define":{"type":"string","min":"0","max":"64"}}]}
class TEMPLATE_CONSTANTS:
    VERSION = "version"
    TYPE = "type"
    NAME = "name"
    ID = "id"
    MIN = "min"
    MAX = "max"
    DEFINE = "define"
    PROPERTIES = "properties"
    EVENTS = "events"
    MAPPING = "mapping"
    UNIT = "unit"
    UNITDESC = "unitDesc"
    REQUIRED = "required"
    MODE = "mode"



class iot_enum:
    def __init__(self, parent, name, index):
        self.parent = parent
        self.id = name
        self.index = index

    def get_c_macro_name(self):
        return f"e_{self.parent.upper()}_{self.id.upper()}"

    def get_define_str(self):
        return f"#define {self.get_c_macro_name()} {self.index}"

class iot_field:
    def __init__(self,id, name, index, field_obj):
        self.default_value = ""
        self.enums = []
        self.index = index
        self.id = id
        self.name = name
        self.type_name = field_obj["define"]["type"]

        if self.type_name == "bool":
            self.type_define = "TYPE_DEF_TEMPLATE_BOOL"
            self.type_id = "TYPE_TEMPLATE_BOOL"
            self.default_value = "0"
        elif self.type_name == "enum":
            self.type_define = "TYPE_DEF_TEMPLATE_ENUM"
            self.type_id = "TYPE_TEMPLATEENUM"
            if TEMPLATE_CONSTANTS.DEFINE not in field_obj:
                raise ValueError(f"错误：{name} 字段定义中未找到枚举定义{TEMPLATE_CONSTANTS.DEFINE} 字段")

            enum_defs = field_obj["define"]["mapping"]


            for enum_id in enum_defs:
                enum_name = enum_defs[enum_id]
                current_enum = iot_enum(self.id, enum_name, enum_id)
                self.enums.append(current_enum)
                if self.default_value == "":
                    self.default_value = enum_id
            if self.default_value == "":
                raise ValueError(f'错误：{name} 字段默认值 {field_obj["default"]} 非法')

        elif self.type_name == "float":
            self.type_define = "TYPE_DEF_TEMPLATE_FLOAT"
            self.type_id = "TYPE_TEMPLATE_FLOAT"

            self.min_value = field_obj["define"]["min"]
            self.max_value = field_obj["define"]["max"]
            self.default_value = field_obj["define"]["start"]
            if float(self.default_value) < float(self.min_value) or float(self.default_value) > float(self.max_value):
                raise ValueError(f"错误：{name} 字段 default 指定的默认值超出 min~max 取值范围")
        elif self.type_name == "int":
            self.type_define = "TYPE_DEF_TEMPLATE_INT"
            self.type_id = "TYPE_TEMPLATE_INT"

            self.min_value = field_obj["define"]["min"]
            self.max_value = field_obj["define"]["max"]
            self.default_value = field_obj["define"]["start"]
            if int(self.default_value) < int(self.min_value) or int(self.default_value) > int(self.max_value):
                raise ValueError(f"错误：{name} 字段 default 指定的默认值超出 min~max 取值范围")
        elif self.type_name == "string":
            self.type_define = "TYPE_DEF_TEMPLATE_STRING"
            self.type_id = "TYPE_TEMPLATE_STRING"

            self.min_value = field_obj["define"]["min"]
            self.max_value = field_obj["define"]["max"]
            self.default_value = "{'\\0'}"
        elif self.type_name == "timestamp":
            self.type_define = "TYPE_DEF_TEMPLATE_TIME"
            self.type_id = "TYPE_TEMPLATE_TIME"
            self.default_value = 0
        else:
            raise ValueError(
                f'{name} 字段 数据类型 type={field_obj["type"]} 取值非法，有效值应为：bool,enum,int,float,string'
            )

    def get_id_c_macro_name(self):
        return f"TC_IOT_PROP_{self.id}"

    def get_id_c_member_name(self):
        return f"m_{self.id}"

    def get_id_default_value(self):
        return f"{self.default_value}"

    def get_id_define_str(self):
        return f"#define {self.get_id_c_macro_name()} {self.index}"

    def get_struct_field_declare(self):
        if self.type_id == "TYPE_TEMPLATE_STRING":
            return f"TYPE_DEF_TEMPLATE_STRING m_{self.id}[{str(self.max_value)}+1];"
        else:
            return f"{self.type_define} m_{self.id};"

    def get_global_field_declare(self):
        if self.type_id == "TYPE_TEMPLATE_STRING":
            return "TYPE_DEF_TEMPLATE_STRING sg_{}[{}+1]={};".format(self.id, str(self.max_value),"{0}")
        else:
            return f"{self.type_define} sg_{self.id} = {self.default_value};"

    def get_meta_define_str(self, var_name):
        return '{{ "{}", &{}.{}, {} }},' \
                    .format(self.id, var_name, self.get_id_c_member_name(), self.type_id)
class iot_event:
    def __init__(self,id, name, index, event):
        self.index = index
        self.id = id
        self.name = name
        self.event_type = event["type"]
        self.desc = event["desc"]
        self.event_properties = []
        self.event_property_count = 0

        for property in event["params"]:
            self.event_properties.append(iot_field(property["id"], property["name"], self.event_property_count, property))
            self.event_property_count += 1


    def get_sigle_event_info(self):
        event_info = (
            f'\n   id:{self.id}  name:\"{self.name}\" type:\"{self.event_type}\"\n'
        )

        event_info += f"   property_count:{self.event_property_count} \n   params:["

        for field in self.event_properties:
            event_info += f"\n      para:{field.id}  type:{field.type_id}"

        event_info += "\n   ]"
        return event_info

    def gen_sigle_event_info(self):
        resault = ""
        event_para_info = ""
        event_property_info = ""
        event_var_info = ""
        for field in self.event_properties:
            event_para_info += f"static {field.get_global_field_declare()}\n"
            event_property_info += "\n   {"
            if field.type_id == "TYPE_TEMPLATE_STRING":
                event_property_info += f'.key = \"{field.id}\", .data = sg_{field.id}, .type = {field.type_id}'

            else:
                event_property_info += f'.key = \"{field.id}\", .data = &sg_{field.id}, .type = {field.type_id}'

            event_property_info += "},"
        event_var_info += f"static DeviceProperty g_propertyEvent_{self.id}[] = "
        resault += event_para_info + event_var_info + "{\n"+event_property_info + "\n};\n"
        return resault



class iot_struct:
    def __init__(self, model):
        self.version = model["version"]
        self.fields = []
        self.field_id = 0
        self.events = []
        self.event_id = 0
        for field_define in model["properties"]:
            if TEMPLATE_CONSTANTS.NAME not in field_define:
                raise ValueError("错误：字段定义中未找到 Name 字段")
            self.fields.append(iot_field(field_define["id"], field_define["name"], self.field_id, field_define))
            self.field_id += 1

        for event in model["events"]:
            if TEMPLATE_CONSTANTS.NAME not in event:
                raise ValueError("错误：字段定义中未找到 Name 字段")
            self.events.append(iot_event(event["id"], event["name"], self.event_id, event))
            self.event_id += 1


    def dump_data_info(self):
        print(f"dump iot struct,counts:{self.field_id}")
        for temp_field in self.fields:
            if temp_field.type_name == "enum":
                print(
                    f"{temp_field.id} {temp_field.type_name} {temp_field.default_value} {temp_field.type_define} "
                )

                print("enums:{")
                for enum in temp_field.enums:
                    print(f"{enum.get_c_macro_name()}  ")
                print("}")
            else:
                print(
                    f"{temp_field.id} {temp_field.type_name} {temp_field.default_value} {temp_field.type_define}\n"
                )
    def dump_event_info(self):
        event_str = f"#define EVENT_COUNTS     ({self.event_id})\n"
        for event_d in self.events:
            event_str += f"{event_d.gen_sigle_event_info()}\n"
        return event_str

    def data_config_macro_define(self, struct_Template="sDataPoint",var_gTemplate="sg_DataTemplate"):
        define_str = (
            "" + "/*-----------------data config start  -------------------*/ \n\n"
        )

        define_str += f"#define TOTAL_PROPERTY_COUNT {self.field_id}\n\n"
        define_str += f"static {struct_Template}    {var_gTemplate}[TOTAL_PROPERTY_COUNT];\n\n"

        return define_str

    def declare_product_data_struct(self, struct_name="ProductDataDefine", var_gname="sg_ProductData"):
        result = ""
        result += f"typedef struct _{struct_name}" + " {\n"
        for field in self.fields:
            result += f"    {field.get_struct_field_declare()}\n"
        result += "} " + struct_name + ";\n\n"
        result += f"static   {struct_name}     {var_gname}" + ";\n\n"
        return result

    def property_data_initializer(self, struct_name="ProductDataDefine", var_gProduct="sg_ProductData", var_gTemplate="sg_DataTemplate"):
        init_str = "" + "static void _init_data_template(void)\n{\n"
        #init_str += "    memset((void *) & {}, 0, sizeof({}));\n".format(var_gProduct, struct_name)

        for count, field in enumerate(self.fields):

            if field.type_define == "TYPE_DEF_TEMPLATE_STRING":
                init_str += "    {}.{}[0] = {};\n".format(var_gProduct, field.get_id_c_member_name(), "'\\0'")
                init_str += f"    {var_gTemplate}[{count}].data_property.data = {var_gProduct}.{field.get_id_c_member_name()};\n"

            else:
                init_str += f"    {var_gProduct}.{field.get_id_c_member_name()} = {field.get_id_default_value()};\n"

                init_str += f"    {var_gTemplate}[{count}].data_property.data = &{var_gProduct}.{field.get_id_c_member_name()};\n"

            init_str += f'    {var_gTemplate}[{count}].data_property.key  = \"{field.id}\";\n'

            init_str += f"    {var_gTemplate}[{count}].data_property.type = {field.type_id};\n\n"

        init_str += "};\n"
        return init_str;

    def gen_data_config(self):
        data_config = f"{self.data_config_macro_define()}"
        data_config += f"{self.declare_product_data_struct()}"
        data_config += f"{self.property_data_initializer()}"
        return data_config

    def gen_event_config(self):
        resault = ""
        event_config = ""
        event_str = ""

        event_config += f"\n#define EVENT_COUNTS     ({self.event_id})\n\n"
        events_var = "" + "\nstatic sEvent g_events[]={\n"
        for event_d in self.events:
            event_config += f"{event_d.gen_sigle_event_info()}\n"
            event_str += "\n    {"
            event_str += f'\n     .event_name = \"{event_d.id}\",'
            event_str += f'\n     .type = \"{event_d.event_type}\",'
            event_str += "\n     .timestamp = 0,"
            event_str += f"\n     .eventDataNum = sizeof(g_propertyEvent_{event_d.id})/sizeof(g_propertyEvent_{event_d.id}[0]),"

            event_str += f"\n     .pEventData = g_propertyEvent_{event_d.id},"
            event_str +="\n    },"
        resault += event_config + events_var + event_str + "\n};\n"
        return resault


def main():
    parser = argparse.ArgumentParser(description='Iothub datatemplate and events config code generator.', usage='use "./codegen.py -c xx/config.json" gen config code')
    parser.add_argument('-c','--config', dest='config',metavar='xxx.json', required=False,default='xxx.json',
                        help='copy the generated file (data_config.c and events_config.c) to datatemplate_sample dir '
                             'or your own code dir with datatemplate. '
                              '\nconfig file can be download from tencent iot-hub platfrom. https://console.cloud.tencent.com/iotcloud')
    parser.add_argument('-d','--dest', dest='dest', required=False,default='.',
                        help='Dest directory for generated code files, no / at the end.')

    args = parser.parse_args()

    config_path = args.config
    if not os.path.exists(config_path):
        print(u"错误：配置文件不存在，请重新指定数据模板配置文件路径,请参考用法 ./codegen.py -c xx/data_template.json".format(config_path))
        return 1

    if config_dir := os.path.dirname(config_path):
        config_dir += "/"

    f = open(config_path, "r")
    try:
        thingmodel = json.load(f)
        if 'properties' not in thingmodel:
            thingmodel.properties = []

        if 'events' not in thingmodel:
            thingmodel.events = []

        print(f"加载 {config_path} 文件成功")
    except ValueError as e:
        print(f"错误：文件格式非法，请检查 {config_path} 文件是否是 JSON 格式。")
        return 1

    if TEMPLATE_CONSTANTS.PROPERTIES not in thingmodel:
        print(f"错误：{config_path} 文件中未发现 DataTemplate 属性字段，请检查文件格式是否合法。")
        return 1

    try:
        snippet = iot_struct(thingmodel)

        output_data_config_file_name = f"{args.dest}/data_config.c"
        with open(output_data_config_file_name, "w") as output_file:
            output_file.write(f"{snippet.gen_data_config()}")
        output_event_config_file_name = f"{args.dest}/events_config.c"
        with open(output_event_config_file_name, "w") as output_file:
            output_file.write(
                f"#ifdef EVENT_POST_ENABLED\n{snippet.gen_event_config()}\n#endif\n"
            )

        print(f"文件 {output_data_config_file_name} 生成成功")
        print(f"文件 {output_event_config_file_name} 生成成功")

        return 0
    except ValueError as e:
        print(e)
        return 1


if __name__ == '__main__':
    sys.exit(main())

