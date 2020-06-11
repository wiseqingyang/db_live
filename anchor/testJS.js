var a = {
    billboard_connect_req: 5001, //主播端OBS广告牌消息
    /*{
    	msgid,
    	roomid,
    }*/
    billboard_connect_ret: 5002,
    /*{
       msgid,
       errcode:0,
       errmsg:"ok",   错误描述.
    }*/

    billboard_msg_notify: 5003, //广告牌的消息通知.
    /*
    {
        errcode: 0,
        errmsg: "ok",
        msgid: 8001, //消息ID
        date_no: 2018010211, //期号
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
}
var groupinfo = {
    msgid: 5003,
    "issue": "2018092401",
    "groupcount": "2",
    "orderamount": "2",
    "wintype": "2",
    "wintypedesc": "多者全拿",
    "status": "1",
    "statusdesc": "匹配中",
    "bettingarr": [{
            "orderid": "20180924011207287531018",
            "bettingid": 1018,
            "bettingname": "1234567_g4111111",
            "tickename": "棒棒糖",
            "facevalue": 1,
            "ticketamount": 1,
            "bonus": 0,
            unfinishamount:9,
            "winamount": 0
        },
        {
            "orderid": "20180924011212598351018",
            "bettingid": 1018,
            "bettingname": "1234567_g4",
            "tickename": "棒棒糖",
            "facevalue": 1,
            "ticketamount": 1,
            "bonus": 0,
            unfinishamount:9,
            "winamount": 0
        }
    ]
}

var rankDatas = [{
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
    {
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
    {
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
    {
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
    {
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
    {
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
    {
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
    {
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
    {
        ticketName: '甜蜜蜜',
        userName: 'mathType',
        awardNum: 10000,
    },
]