window.onload = function () {
    var urlDomain = 'https://test.dbljoy.com/';
    var roomid = 102;

    rootVue = new Vue({
        el: '#rootvue',
        data: {
            websocket: "",
            titleImage: ['./src/groupbig_today.png', './src/singlebig_today.png', './src/groupbig.png', './src/singlebig.png', ],
            ranklistList: [
                [],
                [],
                [],
                [],
            ],
            currentRankIndex: 0,
            currentMatchIndex1: 0,
            currentMatchIndex2: 0,
            timer1: "",
            timer2: "",
            timer3: "",
            groupInfosModelTemp: [],
            groupInfosModels: [
                [],
                []
            ],
        },
        components: {
            'ranklist': {
                template: '#ranklisttmp',
                props: ['titleimage', 'datas', 'index']
            },
            'grouplist': {
                template: '#groupBuyTemplate',
                props: ['groupinfo']
            },
            'groupplaceholder': {
                template: "#groupPlaceholder",
            }
        },
        created() {
            //页面刚进入时开启长连接
            this.initWebSocket()
            this.$nextTick(() => {
                this.timer1 = setInterval(() => {
                    this.autoPlay1()
                }, 8000);
                this.timer2 = setInterval(() => {
                    this.autoPlay2()
                }, 7000);
                this.timer3 = setInterval(() => {
                    this.autoPlay3()
                }, 7000);
            })
            this.initHttpRequests();
            setInterval(() => {
                this.initHttpRequests();
            }, 60000);

        },
        destroyed: function () {
            //页面销毁时关闭长连接
            this.websocket.close();
        },
        methods: {
            websocketonmessage(e) { //数据接收 
                const msgtab = JSON.parse(e.data);
                console.log(JSON.stringify(msgtab));
                var msgid = msgtab.msgid;
                if (msgid == billboard_connect_ret) {
                    if (msgtab.errcode == 0) {
                        console.log("🙋握手成功！");
                    } else {
                        console.log("握手失败");
                    }
                } else if (msgid == billboard_msg_notify) {
                    //合买信息更新。
                    if (!this.checkLocalModel(msgtab)) {
                        // //test code...
                        // msgtab.winnerNum = 1;
                        // msgtab.totalAward = 1000;
                        // for (var i = 0; i < msgtab.bettingarr.length; i++) {
                        //     msgtab.bettingarr[i].usericon = "https://" + msgtab.bettingarr[i].usericon;
                        // }
                        // //test over...
                        this.pushbackNewInfo(msgtab);
                    }
                }
            },
            initWebSocket() { //初始化websocket

                const wsurl = "ws://test.dbljoy.com:2000"; //ws地址
                // const wsurl = "wss://echo.websocket.org";
                this.websocket = new WebSocket(wsurl);
                this.websocket.onopen = this.websocketonopen;
                this.websocket.onerror = this.websocketonerror;
                this.websocket.onmessage = this.websocketonmessage;
                this.websocket.onclose = this.websocketclose;
            },

            initHttpRequests() {
                /*platformfreqlive/getRankingList?buytype=group&timetype=day  合买排行榜
                platformfreqlive/getBigBonusRankingList?timetype=day 单日单张单买 榜单*/
                var self = this;
                //今日合买
                axios.get(urlDomain + 'platformfreqlive/getRankingList', {
                        params: {
                            buytype: 'group',
                            timetype: 'day',
                        }
                    })
                    .then(function (response) {
                        console.log('------------0');
                        console.log(response.data);
                        Vue.set(self.ranklistList, 0, response.data.rankinglist);
                    })
                    .catch(function (error) {
                        console.log(error);
                    });
                //今日大奖
                axios.get(urlDomain + 'platformfreqlive/getBigBonusRankingList', {
                        params: {
                            timetype: 'day',
                        }
                    })
                    .then(function (response) {
                        console.log('------------1');
                        console.log(response.data);
                        Vue.set(self.ranklistList, 1, response.data.rankinglist)
                    })
                    .catch(function (error) {
                        console.log(error);
                    });
                //历史合买
                axios.get(urlDomain + 'platformfreqlive/getRankingList', {
                        params: {
                            buytype: 'group',
                            timetype: 'all',
                        }
                    })
                    .then(function (response) {
                        console.log('------------2');
                        console.log(response.data);
                        Vue.set(self.ranklistList, 2, response.data.rankinglist);
                    })
                    .catch(function (error) {
                        console.log(error);
                    });
                //历史大奖
                axios.get(urlDomain + 'platformfreqlive/getBigBonusRankingList', {
                        params: {
                            timetype: 'all',
                        }
                    })
                    .then(function (response) {
                        console.log('------------3');
                        console.log(response.data);
                        Vue.set(self.ranklistList, 3, response.data.rankinglist);
                    })
                    .catch(function (error) {
                        console.log(error);
                    });
            },

            //检查本地存储情况 返回是否在此函数处理了此组信息
            checkLocalModel(groupInfo) {
                var issue = groupInfo.issue;
                if (groupInfo.status == 4) {
                    //订单取消
                    this.removeMatchInfo(issue);
                    return true;
                } else if (groupInfo.status == 3) {
                    //已完成
                    this.showAwardResult(groupInfo);
                    return true;
                } else {
                    for (var j = 0; j < this.groupInfosModels.length; j++) {
                        for (var i = 0; i < this.groupInfosModels[j].length; i++) {
                            if (this.groupInfosModels[j][i].issue == issue) {
                                if (this.groupInfosModels[j][i].status == 1 && groupInfo.status == 2) {
                                    //原来匹配中 现在开奖中 说明刚开始开奖，
                                    Vue.set(this.groupInfosModels[j], i, groupInfo);
                                    this.moveQueue(j, i);
                                } else {
                                    Vue.set(this.groupInfosModels[j], i, groupInfo);
                                }
                                return true;
                            }
                        }
                    }
                    for (var i = 0; i < this.groupInfosModelTemp.length; i++) {
                        if (this.groupInfosModelTemp[i].issue == issue) {
                            this.groupInfosModelTemp[i] = groupInfo;
                            return true;
                        }
                    }
                    return false;
                }
            },

            pushbackNewInfo(groupInfo) {
                var localModel = this.getRightModel();
                localModel.push(groupInfo);
                if (groupInfo.status == 2) {
                    //开奖中
                    if (localModel.length != 1 && localModel != this.groupInfosModelTemp) {
                        //如果目标Model非空 就执行移动队列操作
                        var index = localModel == this.groupInfosModels[0] ? 0 : 1;
                        this.moveQueue(index, this.groupInfosModels[index].length - 1);
                    }
                }

            },

            getRightModel() {
                var is1Free = this.isViewFree(this.groupInfosModels[0]);
                var is2Free = this.isViewFree(this.groupInfosModels[1]);
                if (is1Free && is2Free) {
                    //都可用 返回元素少的
                    return this.groupInfosModels[0].length <= this.groupInfosModels[1].length ? this.groupInfosModels[0] : this.groupInfosModels[1];
                } else if (is1Free) {
                    //返回可用
                    return this.groupInfosModels[0];
                } else if (is2Free) {
                    return this.groupInfosModels[1];
                } else {
                    //都不可用，返回暂存数组
                    return this.groupInfosModelTemp;
                }
            },

            isViewFree(viewModel) {
                if (viewModel[0] && (viewModel[0].status == 2 || viewModel[0].status == 3)) {
                    //开奖中 已结束 独占期
                    return false;
                } else {
                    return true;
                }
            },

            removeMatchInfo(issue) {
                for (var j = 0; j < this.groupInfosModels.length; j++) {
                    for (var i = 0; i < this.groupInfosModels[j].length; i++) {
                        console.log('new issue:' + issue + ' old issue:' + this.groupInfosModels[j][i].issue)
                        if (this.groupInfosModels[j][i].issue == issue) {
                            const status = this.groupInfosModels[j][i].status;
                            this.groupInfosModels[j].splice(i, 1);
                            if (status == 3) {
                                //开完奖的
                                this.reQueueInfos(j);
                            }
                            return true;
                        }
                    }
                }

                for (var i = 0; i < this.groupInfosModelTemp.length; i++) {
                    if (this.groupInfosModelTemp[i].issue == issue) {
                        this.groupInfosModelTemp.splice(i, 1);
                        return true;
                    }
                }
            },

            showAwardResult(matchInfo) {
                var issue = matchInfo.issue;
                //组织获胜人的数据 人数奖金
                var winnerNum = 0;
                var totalAward = 0;
                var winners = [];
                for (var i = 0; i < matchInfo.bettingarr.length; i++) {
                    if (matchInfo.bettingarr[i].winamount > 0) {
                        winnerNum++;
                        totalAward += matchInfo.bettingarr[i].winamount;
                        winners.push(matchInfo.bettingarr[i]);
                    }
                }
                if (winnerNum == 0) {
                    winnerNum = matchInfo.bettingarr.length;
                    totalAward = 0;
                    winners = matchInfo.bettingarr;
                }
                for (var i = 0; i < winners.length; i++) {
                    winners[i].usericon = "https://" + winners[i].usericon;
                }
                matchInfo.winnerNum = winnerNum; //设置中奖人数，
                matchInfo.totalAward = Math.floor(totalAward * 100) / 100.0;
                matchInfo.winners = winners;
                //更新结果数据
                if (this.groupInfosModels[0][0] && this.groupInfosModels[0][0].issue == issue) {
                    Vue.set(this.groupInfosModels[0], 0, matchInfo);
                } else {
                    Vue.set(this.groupInfosModels[1], 0, matchInfo);
                }
                console.log('===>', matchInfo);

                setTimeout(() => {
                    console.log('remove issue', issue);
                    this.removeMatchInfo(issue);
                }, 5000);

            },
            //index:第几个数组，subIndex 数组第几个元素
            //移动队列，有一个队列需要独占的时候调用
            moveQueue(index, subIndex) {
                const leftIndex = index == 0 ? 1 : 0;
                var tmp = this.groupInfosModels[index];
                this.groupInfosModels[index] = tmp.splice(subIndex, 1);
                if (this.groupInfosModels[leftIndex][0] && this.groupInfosModels[leftIndex][0].status == 2) {
                    //如果另一个队列有数据 另一个队列开奖中 直接放到temp数组中
                    this.groupInfosModelTemp = this.groupInfosModelTemp.concat(tmp);
                } else {
                    //另一队不是开奖中 直接放到队中
                    this.groupInfosModels[leftIndex] = this.groupInfosModels[leftIndex].concat(tmp);
                }
            },

            //重组队列，有一个队列释放独占的时候
            reQueueInfos(index) {
                console.log('queue index: ' + index);
                const leftIndex = index == 0 ? 1 : 0;
                if (this.groupInfosModels[leftIndex][0]) {
                    if (this.groupInfosModels[leftIndex][0].status == 2) {
                        //开奖中 不参与重新排队
                        if (this.groupInfosModelTemp.length > 0) {
                            //移除缓存队列信息
                            var tmpArr = this.groupInfosModelTemp.splice(0, this.groupInfosModelTemp.length);
                            //添加到释放独占的队列
                            this.groupInfosModels[index] = this.groupInfosModels[index].concat(tmpArr);
                            if (this.groupInfosModels[index][0].status == 2) {
                                //新队列首位还是开奖中  挂起独占
                                this.moveQueue(index, 0);
                            }
                        }
                    } else {
                        if (this.groupInfosModels[leftIndex].length > 1) {
                            const number = Math.ceil(this.groupInfosModels[leftIndex].length / 2);
                            if (number >= 1) {
                                var array = this.groupInfosModels[leftIndex].splice(this.groupInfosModels[leftIndex].length - number);
                                this.groupInfosModels[index] = this.groupInfosModels[index].concat(array);
                            }
                        }
                    }
                }
                this.currentMatchIndex1 = 0;
                this.currentMatchIndex2 = 0;
            },

            change(index) {
                this.currentRankIndex = index;
            },
            autoPlay1() {
                this.currentRankIndex++;
                if (this.currentRankIndex > this.ranklistList.length - 1) {
                    this.currentRankIndex = 0
                }
            },
            autoPlay2() {
                this.currentMatchIndex1++;
                if (this.currentMatchIndex1 > this.groupInfosModels[0].length - 1) {
                    this.currentMatchIndex1 = 0
                }
            },
            autoPlay3() {
                this.currentMatchIndex2++;
                if (this.currentMatchIndex2 > this.groupInfosModels[1].length - 1) {
                    this.currentMatchIndex2 = 0
                }
            },

            websocketonopen() {
                console.log("WebSocket连接成功");
                var msgtab = {
                    msgid: billboard_connect_req,
                    roomid: roomid,
                }
                this.websocketsend(JSON.stringify(msgtab));
            },
            websocketonerror(e) { //错误
                console.log("WebSocket连接发生错误");
                setTimeout(() => {
                    if (this.websocket.readyState != 3) {
                        this.initWebSocket();
                    }

                }, 5000)
            },
            websocketsend(agentData) { //数据发送 
                this.websocket.send(agentData);
            },

            websocketclose(e) { //关闭 
                console.log("connection closed (" + e.code + ")");
                setTimeout(() => {
                    if (this.websocket.readyState != 3) {
                        this.initWebSocket();
                    }

                }, 5000)
            },
        },
    })
}