billboard_connect_req = 5001; //主播端OBS广告牌消息
/*{
    msgid,
    roomid,
}*/
billboard_connect_ret = 5002;
/*{
   msgid,
   errcode:0,
   errmsg:"ok",   错误描述.
}*/

billboard_msg_notify = 5003; //广告牌的消息通知.
/*
{
    errcode: 0,
    errmsg: "ok",
    msgid: 8001, //消息ID
    issue: 2018010211, //期号
    betting_amount: 9, // 购买人数
    order_amount: 900, //订单总金额
    win_type_desc: "多者全拿", /方式  ,1奖金平分 , 2多者全拿 , 3少者全拿,
    status: 1, //订单状态  1 匹配中 2 开奖中 3 已完成 4 订单取消
    status_desc: "匹配中", //订单状态  1 匹配中 2 开奖中 3 已完成 4 订单取消
    betting_item: [{
            betting_id: 1018, //购买者ID
            betting_name: "哈哈哈", //购买者昵称 ,
            ticket_name: "财神来了", //彩票名字
            face_value: 10, //彩票面值
            ticket_amount: 10, //总张数量
            unfinish_amount: 9, //   剩余未开数量
            win_amount: "累计中奖金额" //已开全部中奖金额
        }
    ]
}

*/

billboard_msg_result_notify = 5004; //合买开奖结果通知.