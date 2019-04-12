#ifdef _WIN32
#include <filesystem>
namespace fs = std::tr2::sys;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif
#include <iostream>
#include <random>

int divede(const double *rate,   //�ָ������rate[0]Ϊѵ�����ݱ�����rate[1]Ϊ�������ݱ���
	const std::string &srcPath,		//��Ҫ�ָ������path,ע��Ĭ��Ϊpath�»������������ݵ�����Ŀ¼���� people1��people2,car1,car2,car3��,������ͼ���ļ���xml�ļ�
	const std::string &trainPath,  //��ŷָ�õ���ѵ�����ݵ�Ŀ¼
	const std::string &testPath,			//��ŷָ�õ��Ĳ������ݵ�Ŀ¼
	const std::string &extension = ".jpg"		//ͼ����չ��
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
	std::cout << "[\n"<< trainPath <<"\n"<< testPath <<"\n]�������.."<< std::endl << std::endl;

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
	std::cout << "[\n" << trainPath << "\n" << testPath << "\n]�µ�category�ļ��д������..\n" << std::endl;
	std::cout << "���� " << filenames.size() << " ������\n" << std::endl;

	std::cout << "��ʼ��������..\n" << std::endl;
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
			std::cout << "����� [ " << i / progress_count << "% ].." << std::endl;
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

	std::cout << "���ݻ�����ɣ�\n" << std::endl;
	std::cout << "Please check divided data in\n" << trainPath << "\n" << testPath << std::endl;

	system("pause");
	return EXIT_STATUS;
}