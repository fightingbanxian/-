//index.js
//获取应用实例
const app = getApp()

Page({
  data: {
    test: "",
    motto: '熬夜伤身体',
    userInfo: {},
    hasUserInfo: false,
    canIUse: wx.canIUse('button.open-type.getUserInfo')
  },
  //事件处理函数
  bindViewTap: function() {
    wx.navigateTo({
      url: '../logs/logs'
    })
  },
  onLoad: function () {
    if (app.globalData.userInfo) {
      this.setData({
        userInfo: app.globalData.userInfo,
        hasUserInfo: true
      })
    } else if (this.data.canIUse){
      // 由于 getUserInfo 是网络请求，可能会在 Page.onLoad 之后才返回
      // 所以此处加入 callback 以防止这种情况
      app.userInfoReadyCallback = res => {
        this.setData({
          userInfo: res.userInfo,
          hasUserInfo: true
        })
      }
    } else {
      // 在没有 open-type=getUserInfo 版本的兼容处理
      wx.getUserInfo({
        success: res => {
          app.globalData.userInfo = res.userInfo
          this.setData({
            userInfo: res.userInfo,
            hasUserInfo: true
          })
        }
      })
    }
  },
  getUserInfo: function(e) {
    console.log(e)
    app.globalData.userInfo = e.detail.userInfo
    this.setData({
      userInfo: e.detail.userInfo,
      hasUserInfo: true
    })
  }

  ,send1: function () {
    var that = this //创建一个名为that的变量来保存this当前的值  
    wx.request({
      url: 'http://api.heclouds.com/devices/505222423/datapoints?type=3',
      method: 'post',
      data: {
        kaiguan: '1'  //这里是发送给服务器的参数（参数名：参数值）  
      },
      header: {
        'content-type': 'application/json',
        'api-key': 'KPGddm68DGn35VA=p=Llg78AJE8='
      },
      success: function (res) {
        that.setData({ //这里是修改data的值  
          test: res.data //test等于服务器返回来的数据  
        });
        console.log(res.data)
      }
    })


    setTimeout(function () {
      //  var that = this //创建一个名为that的变量来保存this当前的值  
      wx.request({
        url: 'http://api.heclouds.com/devices/505222423/datapoints?type=3',
        method: 'post',
        data: {
          kaiguan: '0'  //这里是发送给服务器的参数（参数名：参数值）  
        },
        header: {
          'content-type': 'application/json',
          'api-key': 'KPGddm68DGn35VA=p=Llg78AJE8='
        },
        success: function (res) {
          that.setData({ //这里是修改data的值  
            test: res //test等于服务器返回来的数据  
          });
          console.log(res.data)
        }
      }) //你需要执行的代码
    }, 12000)

  }
})
