#pragma once

/*
#define MSG_NOTIFY 1000
#define MSG_NEW_ORDER_NOTIFY    MSG_NOTIFY + 1		//�¶�����Ϣ

#define MSG_REQ_RET 2000
#define MSG_AWARD_INFO_REQ      MSG_REQ_RET + 1		//���������
#define MSG_AWARD_INFO_RET      MSG_REQ_RET + 2*/

#define heart_beating 1000 //����

#define join_room_req  1001 //���ӽ��뷿�� {msgid, roomid}
#define join_room_ret  1002 //���ӽ��뷿�� {msgid, roomid}
/*
msgid:1000,    //socket ��Ϣ�����С�
	roomid,
	account,
	seatindex
*/

#define award_result_req 1003 //������{msgid, ticketcode, orderid, operatorid }
#define award_result_ret 1004 //������{msgid, ret:0, des:"ok", bonus:5}  ���δ�н����ǽ���0.



#define beting_notify_to_operator 2001 //�¶���
/*
{
	ticketicon,//��Ʊicon
		ticketname,//��Ʊ����
		ticketid,//��Ʊid
		oderid,//������
		userhead,//��ͷͷ��
		username,//�û�����
		account,//�û�id
		buytime,//����ʱ��
		buynum,//��������
		isgroup,//�Ƿ����
}*/

#define manager_lottery_notify 2002   //����Ա֪ͨ��ƱԱ�Ľ���.
	/*
	bonus,    //�н����
	ticket_code, //��Ʊ����
	order_id, //��ˮ��
	*/