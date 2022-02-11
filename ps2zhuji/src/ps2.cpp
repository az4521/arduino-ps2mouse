/* 
 *�ң���į��ŷ�ὴ������Synaptics PS/2 TouchPad Interfacing Guide�ϵ�ʾ�����룬
 *��ʾ�������ԭ����ps2��ϲ���������һЩ���ģ��������ʺ�arduino��ʹ���Լ������ҵ�ϰ�ߡ�
 *ps/2Э���������˺��豸�˲�����ȫһ������ͨѶʱ�в��
 *������Ϊps/2��������ʹ�ã�������ps/2�豸ͨѶ�� 
 *����Synaptics PS/2 TouchPad Interfacing Guide�ϵ����������ذ�ÿ�뷢��40������80�����ݰ�����������ݴ�������Ӧ���㹻�ˡ�
 *��ȡ�ֽڵ���żУ�����ѡ�����ǲ�������������żУ��Ͱ���ش���ע�͵���Ĭ���ǲ�����
 *��ȡ����д��ʱ��������ʱ������صĴ��뱨����󣬾��忴������ע�ͣ�Ĭ����ע�͵���
 */
#include "ps2.h"
//arduino����ȷ����ʱ�����ź��������ŵ�״̬����˲���Ҫ������������������������
//PS/2�豸�ĵ�Դ���źͽӵ��������ӵ����������Ӧλ�ã�����������Դ��
PS2::PS2(int clock, int data)
{
	ps2clock = clock;
	ps2data = data;
	high(ps2clock);
	high(ps2data);
}
//�������Ÿߵ�ƽ
void PS2::high(int pin)
{
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
}

//�������ŵ͵�ƽ
void PS2::low(int pin)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

//��PS/2�豸д��һ���ֽ�
void PS2::write(unsigned char data)
{
	unsigned char i;
	unsigned char parity = 1;
	unsigned char ack;
	
	//����ps/2�豸ͨѶ
	low(ps2clock);
	delayMicroseconds(100);
	//����ps/2�豸ͨѶ
	low(ps2data);
	//������ʼλ��
	high(ps2clock);	
	//�������ݡ�
	for (i=0; i < 8; i++)
	{
		//�ȴ�ps/2�豸ʱ�����塣
		while (digitalRead(ps2clock) != LOW);
		//�����i������λ��
		if (data & 0x01)
		{
			high(ps2data);
		} else {
			low(ps2data);
		}
		parity = parity + data;
		//�ȴ�ps/2�豸ʱ�����������
		while (digitalRead(ps2clock) != HIGH);
		data = data >> 1;
	}
	while (digitalRead(ps2clock) != LOW);
	//������żУ��λ��
	if (parity & 0x01)
	{
		high(ps2data);
	} else {
		low(ps2data);
	}
	//�ȴ���ż����λʱ�ӽ�����
	while (digitalRead(ps2clock) != HIGH);
	//ֹͣλʱ�ӿ�ʼ
	while (digitalRead(ps2clock) != LOW);
	//����ֹͣλ��ֹͣλ����Ϊ1����
	high(ps2data);
	//�ȴ�ֹͣλʱ�ӽ�����
	while (digitalRead(ps2clock) != HIGH);
	//��·����ʱ�ӿ�ʼ��
	while (digitalRead(ps2clock) != LOW);
	//����Ǳ����߿���λ����Ĵ��룬Ĭ����ע�͵��ⲿ�ִ��롣
	/*
	if (digitalRead(ps2clock) == 1){
		//����ķ������ޣ������ǵ���һ��LED��Ҳ�����Ǵ��ڷ����ַ���
		//������ʹ�ô��ڷ����ַ��ķ�ʽ��ռ�ڴ�Ƚ϶ࡣ
		serial.print("д���ֽ�ʱȱ���߿���λ��\n");
	}
	*/
	//�ȴ���·����ʱ�ӽ�����
	while (digitalRead(ps2clock) != HIGH);
	//���ps/2�豸�Ƿ�����ֽڣ�Ĭ����ע�͵��ⲿ�ִ��롣
	//ps/2�������������ʱ�᷵��һ��FA�������ֽڳ���ʱ���᷵�������ֽڡ�
	//������һ�д��븺����շ��ص��ֽڣ������Ͳ���Ҫ����Ŀ�ֶ������ֽڡ�
	ack = read();
	//У����յ����ֽڣ��������FA���ͱ�������������Ը��ݽ��յ��ֽ��ж��������ڡ�
	//����ķ������ޣ������ǵ���һ��LED��Ҳ�����Ǵ��ڷ����ַ���
	//������ʹ�ô��ڷ����ַ��ķ�ʽ��ռ�ڴ�Ƚ϶ࡣ
	//Ĭ����ע�͵��ġ�
	/*
	if (ack != 0xFA){
		serial.print(ack,HEC);
	}
	*/
}
//��ps2�豸��ȡһ���ֽڡ�
unsigned char PS2::read(void)
{
	unsigned char i;
	unsigned char bit;
	unsigned char data = 0x00;
	//������ż����ı�����Ĭ��ע�͵�
	//unsigned char parity = 0;
	//ps/2ͨѶ���ƽ����
	high(ps2clock);
	//�ȴ�ps/2�豸������ʼλ��
	while (digitalRead(ps2clock) != LOW);
	//��ʼλ���ͽ�����
	while (digitalRead(ps2clock) != HIGH);	
	for (i=0; i < 8; i++)
	{
		//�ȴ�ps/2�豸��ʱ�����塣
		while (digitalRead(ps2clock) != LOW);
		bit = digitalRead(ps2data);
		data = data + (bit << i);
		//����żУ��λ�ۼ�Ϊ��żУ��λ��Ĭ��ע�͵���
		//parity = parity + bit;
		//�ȴ�ps/2�豸��ʱ�����塣
		while (digitalRead(ps2clock) != HIGH);
	}
	//��żУ��λʱ�ӿ�ʼ��
	while (digitalRead(ps2clock) != LOW);
	//����żУ��λ�ۼ�Ϊ��żУ��λ��Ĭ��ע�͵���
	//parity = parity + digitalRead(ps2data);
	//����Ǳ�����ż����λ����Ĵ��룬Ĭ����ע�͵��ⲿ�ִ��롣
	/*
	if ((parity & 0x01) == 0){
		//����ķ������ޣ������ǵ���һ��LED��Ҳ�����Ǵ��ڷ����ַ���
		//������ʹ�ô��ڷ����ַ��ķ�ʽ��ռ�ڴ�Ƚ϶ࡣ
		serial.print("��ȡ�ֽ�ʱ����żУ�����\n");
	}
	*/
	//��żУ��λʱ�ӽ���
	while (digitalRead(ps2clock) != HIGH);
	//ֹͣλʱ�ӡ�
	while (digitalRead(ps2clock) != LOW);
	//����Ǳ���ֹͣλ����Ĵ��룬Ĭ����ע�͵��ⲿ�ִ��롣
	/*
	if (read_DATA() == 0){
	    //����ķ������ޣ������ǵ���һ��LED��Ҳ�����Ǵ��ڷ����ַ���
		//������ʹ�ô��ڷ����ַ��ķ�ʽ��ռ�ڴ�Ƚ϶ࡣ
		serial.print("��ȡ����ʱ��ֹͣλ����\n");
	}
	*/
	//ps/2ͨѶ���ơ�
	low(ps2clock);
	//�ȴ�����ʱ�����塣
	delayMicroseconds(50);
	//���ؽ��յ������ݡ�
	return data;
}

