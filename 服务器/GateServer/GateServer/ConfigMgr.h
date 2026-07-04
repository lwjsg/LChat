#include"const.h"
//创建 ConfigMgr.h，就是为了写一个【全局配置管理器】，让程序能轻松读取、管理、使用 config.ini 里的所有配置。

//SectionInfo类管理key和value
struct SectionInfo {
	SectionInfo() {}
	~SectionInfo()
	{
		_section_datas.clear();
	}

	//拷贝构造
	SectionInfo(const SectionInfo& src);
	//赋值操作
	SectionInfo& operator = (const SectionInfo& src);
	//重载[]实现查找功能
	std::string  operator[](const std::string& key)
	{
		if (_section_datas.find(key) == _section_datas.end()) {
			return "";
		}
		// 这里可以添加一些边界检查  
		return _section_datas[key];
	}

	std::map<std::string, std::string> _section_datas;
};


//ComigMgr管理section和其包含的key与value
//修改为单例类
class ConfigMgr
{
public:
	~ConfigMgr()
	{
		_config_map.clear();
	}
	SectionInfo operator[](const std::string& section)
	{
		if (_config_map.find(section) == _config_map.end()) {
			return SectionInfo();
		}
		return _config_map[section];
	}
	ConfigMgr(const ConfigMgr& src) = delete;
	ConfigMgr& operator=(const ConfigMgr& src) = delete;

	static ConfigMgr& Inst() {
		static ConfigMgr cfg_mgr;
		return cfg_mgr;
	}

private:
	//负责从项目运行目录读取 config.ini 配置文件，解析所有「节（Section）」和「键值对」，并封装到 SectionInfo 实例中，最终存入 ConfigMgr 的全局配置映射表。
	ConfigMgr()
	{
		// 获取当前工作目录  
		boost::filesystem::path current_path = boost::filesystem::current_path();
		// 构建config.ini文件的完整路径  
		boost::filesystem::path config_path = current_path / "config.ini";
		std::cout << "Config path: " << config_path << std::endl;

		// 使用Boost.PropertyTree来读取INI文件  
		boost::property_tree::ptree pt;
		boost::property_tree::read_ini(config_path.string(), pt);


		// 遍历INI文件中的所有section  
		for (const auto& section_pair : pt) {
			const std::string& section_name = section_pair.first;
			const boost::property_tree::ptree& section_tree = section_pair.second;

			// 对于每个section，遍历其所有的key-value对  
			std::map<std::string, std::string> section_config;
			for (const auto& key_value_pair : section_tree) {
				const std::string& key = key_value_pair.first;
				const std::string& value = key_value_pair.second.get_value<std::string>();
				section_config[key] = value;
			}
			SectionInfo sectionInfo;
			sectionInfo._section_datas = section_config;
			// 将section的key-value对保存到config_map中  
			_config_map[section_name] = sectionInfo;
		}

		// 输出所有的section和key-value对  
		for (const auto& section_entry : _config_map) {
			const std::string& section_name = section_entry.first;
			SectionInfo section_config = section_entry.second;
			std::cout << "[" << section_name << "]" << std::endl;
			for (const auto& key_value_pair : section_config._section_datas) {
				std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
			}
		}
	}
	// 存储section和key-value对的map  
	std::map<std::string, SectionInfo> _config_map;
};

//_config_map
//├─ key:"GateServer" → value:SectionInfo
//│                  └─ _section_datas
//│                      └─ key:"Port" = value:"8080"
//│
//└─ key:"VarifyServer" → value:SectionInfo
//                     └─ _section_datas
//                         └─ key:"Port" = value:"50051"


//1. 它里面定义了两个关键类
//SectionInfo
//管理 单个配置节（如[GateServer]）的所有键值对。
//ConfigMgr
//管理 整个配置文件，加载、解析、存储所有节。
//2. 它帮程序做了什么？
//自动找到 config.ini 路径
//不用写死路径，自动在程序运行目录找配置文件。
//自动解析 INI 配置
//把[节] key = value 变成程序能使用的内存结构。
//把配置存到内存里，随时用
//构造函数执行一次，所有配置就都加载进内存了。
//提供超级简单的取值方式
//cpp
//运行
//gCfgMgr["GateServer"]["Port"]
//gCfgMgr["VarifyServer"]["Port"]
//让配置和代码分离
//要改端口、改地址，只改 ini 文件，不用改代码、不用重新编译。
//全局唯一，到处能用
//任何文件只要包含 ConfigMgr.h，就能读取配置。