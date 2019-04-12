#ifdef _WIN32
#include <filesystem>
namespace fs = std::tr2::sys;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif
#include <iostream>
#include <random>

int divede(const double *rate,   //分割比例，rate[0]为训练数据比例，rate[1]为测试数据比例
	const std::string &srcPath,		//需要分割的数据path,注意默认为path下还会有所有数据的种类目录比如 people1，people2,car1,car2,car3等,而不是图像文件和xml文件
	const std::string &trainPath,  //存放分割得到的训练数据的目录
	const std::string &testPath,			//存放分割得到的测试数据的目录
	const std::string &extension = ".jpg"		//图像扩展名
	)
{
	if (!fs::exists(srcPath))
	{
		std::cout << "path[ " << srcPath << " ] doesn't exists." << std::endl;
		return EXIT_FAILURE;
	}

	if (!fs::exists(trainPath))
		fs::create_directory(trainPath);
	if (!fs::exists(testPath))
		fs::create_directory(testPath);
	std::cout << "[\n"<< trainPath <<"\n"<< testPath <<"\n]创建完毕.."<< std::endl << std::endl;

	std::vector<std::string> filenames;

	fs::path path(srcPath);
	fs::directory_iterator endIter;
	for (fs::directory_iterator iter(path); iter != endIter; ++iter)    
	{
		std::string category_file = iter->path().filename().string();
		std::string train_category_file = trainPath + "/" + category_file;
		std::string test_category_file = testPath + "/" + category_file;

		std::cout << "find category " << category_file << std::endl;
		if (!fs::exists(train_category_file))
			fs::create_directory(train_category_file);
		if (!fs::exists(test_category_file))
			fs::create_directory(test_category_file);

		fs::path dirPath(iter->path());
		fs::directory_iterator dirEndIter;
		for (fs::directory_iterator fileIter(dirPath); fileIter != dirEndIter; ++fileIter)
		{
			if (fileIter->path().extension() == extension)
			{
				std::string filename = "/" + category_file + "/" + fileIter->path().filename().string();
				filename.erase(filename.find(extension), extension.size());
				filenames.push_back(filename);
			}
		}
	}
	std::cout << "[\n" << trainPath << "\n" << testPath << "\n]下的category文件夹创建完毕..\n" << std::endl;
	std::cout << "共有 " << filenames.size() << " 条数据\n" << std::endl;

	std::cout << "开始划分数据..\n" << std::endl;
	std::random_shuffle(filenames.begin(), filenames.end());
	size_t size = filenames.size();
	size_t progress_count = size / 100;
	size_t train_num = rate[0] * size;

#ifdef _WIN32
	auto copy_option = fs::copy_options::overwrite_existing;
#else
	auto copy_option = fs::copy_option::overwrite_if_exists;
#endif
	for (int i = 0; i < size; ++i)
	{
		if (i%progress_count == 0)
			std::cout << "已完成 [ " << i / progress_count << "% ].." << std::endl;
		if (i < train_num)
		{
			std::string src_img_path = srcPath + filenames[i]+ extension;
			std::string tar_img_path = trainPath + filenames[i]+ extension;
			std::string src_xml_path = srcPath + filenames[i] + ".xml";
			std::string tar_xml_path = trainPath + filenames[i] + ".xml";
			fs::copy_file(src_img_path, tar_img_path, copy_option);
			fs::copy_file(src_xml_path, tar_xml_path, copy_option);
		}
		else
		{
			std::string src_img_path = srcPath + filenames[i] + extension; 
			std::string tar_img_path = testPath + filenames[i] + extension;
			std::string src_xml_path = srcPath + filenames[i] + ".xml";
			std::string tar_xml_path = testPath + filenames[i] + ".xml";
			fs::copy_file(src_img_path, tar_img_path, copy_option);
			fs::copy_file(src_xml_path, tar_xml_path, copy_option);
		}
	}

	return EXIT_SUCCESS;
}

int main()
{
	int EXIT_STATUS = EXIT_SUCCESS;
	double divide_rate[2]{ 0.8,0.2};
	std::string srcPath = "D:/BaiduNetdiskDownload/data_training_V4/data";
	std::string trainPath = "D:/BaiduNetdiskDownload/data_training_V4/data_train";
	std::string testPath = "D:/BaiduNetdiskDownload/data_training_V4/data_test";

	EXIT_STATUS = divede(divide_rate, srcPath, trainPath, testPath);

	std::cout << "数据划分完成！\n" << std::endl;
	std::cout << "Please check divided data in\n" << trainPath << "\n" << testPath << std::endl;

	system("pause");
	return EXIT_STATUS;
}