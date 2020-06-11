#pragma once

/*
#define MSG_NOTIFY 1000
#define MSG_NEW_ORDER_NOTIFY    MSG_NOTIFY + 1		//新订单消息

#define MSG_REQ_RET 2000
#define MSG_AWARD_INFO_REQ      MSG_REQ_RET + 1		//请求奖励结果
#define MSG_AWARD_INFO_RET      MSG_REQ_RET + 2*/

#define heart_beating 1000 //心跳

#define join_room_req  1001 //连接进入房间 {msgid, roomid}
#define join_room_ret  1002 //连接进入房间 {msgid, roomid}
/*
msgid:1000,    //socket 消息必须有。
	roomid,
	account,
	seatindex
*/

#define award_result_req 1003 //请求奖项{msgid, ticketcode, orderid, operatorid }
#define award_result_ret 1004 //请求奖项{msgid, ret:0, des:"ok", bonus:5}  如果未中奖就是奖金0.



#define beting_notify_to_operator 2001 //新订单
/*
{
	ticketicon,//彩票icon
		ticketname,//彩票名字
		ticketid,//彩票id
		oderid,//订单号
		userhead,//用头头像
		username,//用户名字
		account,//用户id
		buytime,//购买时间
		buynum,//购买数量
		isgroup,//是否合买
}*/

#define manager_lottery_notify 2002   //管理员通知刮票员的奖项.
	/*
	bonus,    //中奖金额
	ticket_code, //彩票编码
	order_id, //流水号
	*/