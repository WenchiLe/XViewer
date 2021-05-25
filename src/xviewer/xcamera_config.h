#pragma once
#include <vector>
#include <mutex>
struct XCameraData
{
	char name[1024] = { 0 };
	char url[4096] = { 0 };			//�����������
	char sub_url[4096] = { 0 };		//�����������
	char save_path[4096] = { 0 };	//��Ƶ¼�ƴ��Ŀ¼

};

class XCameraConfig
{
public:
	//Ψһ����ʵ�� ����ģʽ
	static XCameraConfig* Instance()
	{
		static XCameraConfig xc;
		return &xc;
	}

	//��������� �̰߳�ȫ
	void Push(const XCameraData &data);

	//��ȡ����ͷ
	//index ����ͷ���� ��0��ʼ
	//���� �ն��� �����ж�nameΪ���ַ���
	XCameraData GetCam(int index);

	//��ȡ�������
	//ʧ�ܷ���0
	int GetCamCount();

	//�޸����������
	bool SetCam(int index, const XCameraData& data);

	//ɾ�������
	bool DelCam(int index);

	//�洢�����ļ�
	bool Save(const char* path);

	//�洢���� ����cams
	bool Load(const char* path);

private:
	XCameraConfig() {}		//����ģʽ
	std::vector<XCameraData> cams_;
	std::mutex mux_;
};

