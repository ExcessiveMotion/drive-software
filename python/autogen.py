import yaml, datetime, os

class cFileGenerator:

    def __init__(self, filePath: str) -> None:
        """
        fileName: Directory and name with extension of the file to generate, no spaces allowed
        """

        self.fullFilePath = filePath
        self.fileName = filePath.split("/")[-1]

        self.file = ""

    def define(self, name: str, value: str, comment: str = "") -> None:
        if(comment != ""):
            self.file += f"\n#define {name} {value}\t\t// {comment}"
        else:
            self.file += f"\n#define {name} {value}"

    def include(self, value: str, comment: str = "") -> None:
        if(comment != ""):
            self.file += f"\n#include{value}\t\t// {comment}"
        else:
            self.file += f"\n#include{value}"

    def blankLine(self, count=1, comment: str = "") -> None:
        for i in range(count):
            if(comment != ""):
                self.file += f"\n// {comment}"
            else:
                self.file += f"\n"

    def enum(self, name: str, enums: dict, type: str = "", comment: str = "") -> None:

        self.file += f"\nenum {name}"
        if(type != ""):
            self.file += f": {type}"
        self.file += "{"
        if(comment != ""):
            self.file += f"\t\t// {comment}"
        try:
            self.file += f"\n\t{name}_start = {enums['start']},"
            enums.pop("start")
        except:
            pass
        end = None
        try:
            end = f"\n\t{name}_end = {enums['end']},"
            enums.pop("end")
        except:
            pass

        for enum_index, enum_data in enums.items():
            self.file += f"\n\t{enum_data['name']} = {enum_index},"
            if(enum_data["comment"] != ""):
                self.file += f"\t\t// {enum_data['comment']}"

        if(end is not None):
            self.file += end


        self.file += "\n};"

    def array(self, type: str, name: str, size: int, initValue = None, comment: str = "", newline=False) -> None:
        if(comment != ""):
            self.file += f"\n// {comment}"

        self.file += f"\n{type} {name}[{size}]"
        initList = []

        if(isinstance(initValue, int) or isinstance(initValue, float)):
            initList = [initValue]*size
        else:
            initList = initValue

        self.file += " = {"
        
        for i in initList:
            if newline:
                self.file += "\n\t"
            else:
                self.file += " "

            if(isinstance(i, str)):
                self.file += f"{i},"
            else:
                self.file += f"{i},"

        self.file = self.file[:-1]

        if newline:
            self.file += "\n"
        self.file += "};"
            
    def struct(self, name: str, struct: dict, comment: str = "") -> None:
        self.file += f"\nstruct {name}"
        self.file += "{"

        if(comment != ""):
            self.file += f"\t\t// {comment}"
        
        for var_name, var_info in struct.items():
            self.file += f"\n\t{var_info["type"]} {var_name} = {var_info['value']};"
            self.file += f"\t\t// ({var_info['unit']}) {var_info['description']}"
        
        self.file += "\n};"
    
    def ifDefine(self, defineToCheck: str) -> None:
        self.file += f"\n#ifndef {defineToCheck}"

    def ifDefineEnd(self) -> None:
        self.file += f"\n#endif"

    def saveFile(self) -> None:
        f = open(self.fullFilePath, "w")
        f.write(self.file)
        f.close()



currentDir = os.path.dirname(os.path.abspath(__file__)).replace("\\", "/")
# load descriptor file
try:
    deviceDescriptor = yaml.safe_load(open(currentDir + "/device_descriptor.yaml"))
except yaml.parser.ParserError as e:
    print(f"AUTOGEN: Error while parsing device_descriptor file: {e}")
    raise e
except FileNotFoundError:
    print("AUTOGEN: device_descriptor.yaml not found")
    raise FileNotFoundError


# generate header file for device firmware
deviceFileGen = cFileGenerator(currentDir + "/../firmware/Core/Inc/device_descriptor.h")
deviceFileGen.file = f"""
    //////////////////// AUTO GENERATED FILE ////////////////////

    // do not manually modify this file, modify "device_descriptor.yaml" instead
    // run autogen.py to update (called at each build by default)

    // last updated at {datetime.datetime.now()}

    #pragma once
    #include <stdint.h>

    """

controllerFileGen = cFileGenerator(currentDir + f"/device_{deviceDescriptor['hardware']['type']}_descriptor.h")
controllerFileGen.file = f"""
    //////////////////// AUTO GENERATED FILE ////////////////////

    // this file specifies how the controller should interact with the device

    // do not manually modify this file, modify the "device_descriptor.yaml" associated with the device firmware instead

    // last updated at {datetime.datetime.now()}

    #pragma once
    #include <stdint.h>

    """
controllerFileGen.file += f"namespace em_serial_device_{deviceDescriptor['hardware']['type']} {{\n\n"

deviceFileGen.define("HARDWARE_TYPE", deviceDescriptor["hardware"]["type"])
deviceFileGen.define("HARDWARE_VERSION", deviceDescriptor["hardware"]["version"])
deviceFileGen.define("FIRMWARE_VERSION", deviceDescriptor["firmware"]["version"])
deviceFileGen.blankLine(2)

# define hardware params
for param in deviceDescriptor["hard_parameters"].items():
    singleLineDescription = param[1]['description'].replace('\n', ' ')
    if(param[1]['unit'] != ""):
        deviceFileGen.define(param[0].upper(), param[1]["value"], f"({param[1]['unit']}) {singleLineDescription}")
    else:
        deviceFileGen.define(param[0].upper(), param[1]["value"], f"{singleLineDescription}")
deviceFileGen.blankLine(2)


# generate cyclic register access registers
cyclic_count = deviceDescriptor["hard_parameters"]["cyclic_address_count"]["value"]
cyclic_read_info = deviceDescriptor["registers"].pop("cyclic_read_address_n")
cyclic_write_info = deviceDescriptor["registers"].pop("cyclic_write_address_n")
for index in range(cyclic_count):
    deviceDescriptor["registers"][f"cyclic_read_address_{index}"] = cyclic_read_info
for index in range(cyclic_count):
    deviceDescriptor["registers"][f"cyclic_write_address_{index}"] = cyclic_write_info


# generate struct for device
vars = {
    "int8_t":{},
    "int16_t":{},
    "int32_t":{},
    "uint8_t":{},
    "uint16_t":{},
    "uint32_t":{},
    "float":{}
}
struct_vars = {}
struct_vars["hardware_type"] = {
    "type":"uint32_t",
    "unit":"",
    "value":deviceDescriptor["hardware"]["type"],
    "description":"hardware type",
    "permissions":["read"]
}
struct_vars["hardware_version"] = {
    "type":"uint32_t",
    "unit":"",
    "value":deviceDescriptor["hardware"]["version"],
    "description":"hardware version",
    "permissions":["read"]
}
struct_vars["firmware_version"] = {
    "type":"uint32_t",
    "unit":"",
    "value":deviceDescriptor["firmware"]["version"],
    "description":"firmware version",
    "permissions":["read"]
}
cyclic_read_address_start = None
cyclic_write_address_start = None
cyclic_enable_address = None
i = 3
for register_name, register_info in deviceDescriptor["registers"].items():

    if(register_info["var_type"] not in vars.keys()):      # catch bad variable types
        raise Exception(f"Unknown var_type: {register_info['var_type']} for {register_name} register")
    t = {
        "type":register_info["var_type"],
        "value":register_info["value"],
        "unit":register_info["unit"],
        "description":register_info["description"],
        "permissions":register_info["permissions"]
    }

    if(register_name.startswith("cyclic_read_address_") and cyclic_read_address_start is None):
        cyclic_read_address_start = i
    if(register_name.startswith("cyclic_write_address_") and cyclic_write_address_start is None):
        cyclic_write_address_start = i
    if(register_name == "enable_cyclic_data" and cyclic_enable_address is None):
        cyclic_enable_address = i

    vars[register_info["var_type"]][register_name] = t

    struct_vars[register_name] = t
    i += 1

deviceFileGen.struct("device_struct", struct_vars, "device variables")
deviceFileGen.blankLine(1)
deviceFileGen.file += "\nstatic device_struct vars;"
deviceFileGen.blankLine(1)


pointers = []
data_info = []
read_permissions = []
write_permissions = []
for name, data in struct_vars.items():
    var_data = 0
    pointers.append(f"&vars.{name}")
    if(data["permissions"].count("read") != 0):
        var_data |= 1 << 0

    if(data["permissions"].count("write") != 0):
        var_data |= 1 << 1
    size = 0
    if(data["type"] in ["int8_t", "uint8_t"]):
        size = 1
    elif(data["type"] in ["int16_t", "uint16_t"]):
        size = 2
    elif(data["type"] in ["int32_t", "uint32_t", "float"]):
        size = 4
    var_data |= size << 4
    data_info.append(var_data)

deviceFileGen.define("CYCLIC_READ_ADDRESS_POINTER_START", cyclic_read_address_start)
deviceFileGen.define("CYCLIC_WRITE_ADDRESS_POINTER_START", cyclic_write_address_start)
deviceFileGen.define("CYCLIC_ENABLE_ADDRESS_POINTER", cyclic_enable_address)
deviceFileGen.define("VAR_COUNT", len(pointers), "number of variables")
deviceFileGen.array("static void*", "var_pointers", len(pointers), initValue=pointers, comment="pointers to all variables", newline=True)
#fileGen.array("static const bool", "read_permissions", len(read_permissions), initValue=read_permissions, comment="read permissions for all variables")
#fileGen.array("static const bool", "write_permissions", len(write_permissions), initValue=write_permissions, comment="write permissions for all variables")
deviceFileGen.array("static const uint8_t", "data_info", len(data_info), initValue=data_info, comment="data info for all variables")

# define message enums
deviceFileGen.blankLine(2)
deviceFileGen.blankLine(1, "Define message enums")
severities = deviceDescriptor["message_severities"]
t = {}
for severity_name, severity_value in severities.items():
    t[severity_value] = {"name":severity_name, "comment":""}

deviceFileGen.enum("message_severities", t, "uint8_t", "message severities")

object_id = 0
total_messages = 0
message_values = []
for object_name, object_messages in deviceDescriptor["messages"].items():

    messages_enum = {}
    message_id = 0
    for message_name, message_info in object_messages.items():
        value = (severities[message_info["severity"]] << 28) | (object_id << 12) | message_id
        message_values.append(value)

        messages_enum[message_id+total_messages] = {"name":message_name, "comment":message_info["description"]}
        message_id += 1

    total_messages += message_id

    deviceFileGen.enum(f"{object_name}_messages", messages_enum, "uint32_t", f"{object_name} messages")

    object_id += 1

deviceFileGen.array("inline uint32_t", "message_values", total_messages, initValue=message_values, comment="message values")

deviceFileGen.define("MESSAGE_COUNT", total_messages, "total number of messages")

deviceFileGen.saveFile()
controllerFileGen.saveFile()

print("\n\nAUTOGEN: Successfully updated auto generated files\n\n")


