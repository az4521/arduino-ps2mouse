/*ps/2Э�������˵Ŀ⡣
 *Chris J.Kiick׫д��2008��1�·�������������
 *Gene E.Scogin���Σ�2008��8�·�������������
 *2022��2��10���ɼ�į��ŷ�ὴ�����޸ģ���ɾ��ע�ͣ���һ������ע��ԭ�ķ������ģ�����������GitHub��
 */
#ifndef ps2_h
#define ps2_h
#include "arduino.h"

class PS2
{
	public:
		PS2(int clock, int data);
		void write(unsigned char data);
		unsigned char read(void);
	private:
		int ps2clock;
		int ps2data;
		void low(int pin);
		void high(int pin);
};
#endif

