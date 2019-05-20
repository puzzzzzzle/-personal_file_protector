import QtQuick 2.9
import filelist 1.0
import Qt.labs.platform 1.0
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2


ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title:qsTr("个人文件加密系统")
    MessageDialog {
        id:errorCommon
        title: "失败"
        icon: StandardIcon.Critical
        text: "用户已经从在或用户名密码不和规范"
        standardButtons: StandardButton.Cancel
        Component.onCompleted: visible = false
    }

    MessageDialog {
        id:okMsgBox
        title: "成功"
        icon: StandardIcon.Information
        text: "成功！"
        standardButtons: StandardButton.Cancel
        Component.onCompleted: visible = false
    }
    //注册
    Dialog {
        id: register
        title: "注册"
        width: 200
        height: 150

        Column {
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            spacing: 15
            Row {
                width: parent.width
                height: 30
                Text {
                    width: 60
                    height: 30
                    //                    anchors.centerIn: parent
                    text: qsTr("name：")
                    font.pixelSize: 15
                }

                TextField {
                    id:nameReg
                    width: parent.width-80
                    height: 30
                    text: qsTr("")
                    font.pixelSize: 15
                }
            }
            Row {
                width: parent.width
                height: 30

                Text {
                    width: 60
                    height: 30
                    text: qsTr("passwd:")
                    font.pixelSize: 15
                }

                TextField {
                    id:passwdReg
                    width: parent.width-80
                    height: 30
                    text: qsTr("")
                    font.pixelSize: 15
                }
            }
        }

        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            if(nameReg.text==""
                    ||passwdReg.text==""){
                console.log("register err in gui")
                errorCommon.text="用户名密码不规范"
                errorCommon.open();
            }

            else if(ctl.registerUser(nameReg.text,passwdReg.text)){
                console.log("register err in gui")
                errorCommon.text="用户已经从在"
                errorCommon.open();
            }
            else{
                okMsgBox.open();
            }
        }
    }

    //登录
    Dialog {
        id: login
        title: "登录"
        width: 200
        height: 150

        Column {
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            spacing: 15
            Row {
                width: parent.width
                height: 30
                Text {
                    width: 60
                    height: 30
                    text: qsTr("name：")
                    font.pixelSize: 15
                }

                TextField {
                    id:nameLogin
                    width: parent.width-80
                    height: 30
                    text: qsTr("")
                    font.pixelSize: 15
                }
            }
            Row {
                width: parent.width
                height: 30

                Text {
                    width: 60
                    height: 30
                    text: qsTr("passwd:")
                    font.pixelSize: 15
                }

                TextField {
                    id:passwdLogin
                    width: parent.width-80
                    height: 30
                    text: qsTr("")
                    font.pixelSize: 15
                }
            }
        }

        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            if(nameLogin.text==""
                    ||passwdLogin.text==""){
                console.log("login err in gui")
                errorCommon.text="用户名密码不规范"
                errorCommon.open();
            }

            else if(ctl.login(nameLogin.text,passwdLogin.text)){
                console.log("login err in gui")
                errorCommon.text="登录失败，用户名或密码错误"
                errorCommon.open();
            }else{
                changeCurrPwdGui()
            }
        }
    }
    menuBar: MenuBar {
        Menu {
            title: qsTr("&文件")
            MenuItem {
                text: "&添加文件"
            }
            MenuItem {
                text: "&添加文件密钥"
            }
            MenuSeparator { }
            MenuItem {
                text: "&启动网络共享"
            }
            MenuSeparator { }
            MenuItem {
                text: "&登录"
                onTriggered:{
                    login.open()
                }
            }
            MenuSeparator { }
            MenuItem {
                text: "&退出"
                shortcut: "Ctrl+Q"
                onTriggered: {
                    Qt.quit();
                }

            }
            MenuItem {
                text: "&登出"
            }
            MenuItem {
                text: "&注册"
                onTriggered:{
                    register.open()
                }
            }
        }
        Menu {
            title: qsTr("&编辑")
            MenuItem {
                text: "&剪切"
            }
            MenuItem {
                text: "&复制"
            }
            MenuItem {
                text: "&粘贴"
            }
            MenuItem {
                text: "&移动"
            }
        }
        Menu {
            title: qsTr("&帮助")
            MenuItem {
                text: "&关于"
            }
        }
    }
    toolBar: ToolBar{
        height: 30
        Row{                //横向
            anchors.fill: parent
            ToolButton{
                iconSource: "up.png"
                height: parent.parent.height
                width:parent.parent.height

            }
            ToolButton{
                height: parent.parent.height
                width:parent.parent.height
                iconSource: "down.png"
            }
            TextField{
                id:pwdGui
                width:parent.width-70
                height: parent.height
                text: "/"
            }
            ToolButton{
                height: parent.parent.height
                width:parent.parent.height
                iconSource: "go.png"
            }
        }
    }

    //listview 逻辑
    ScrollView {
        anchors.fill: parent
        ListView {
            id : m_listView
            anchors.fill: parent
            anchors.margins: 0
            clip: true

            model: mdata
            delegate: numberDelegate
            spacing: 0
            focus: true
            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                onActiveChanged: {
                    active = true;
                }
                Component.onCompleted: {
                    scrollBar.handle.color = "#686A70";
                    scrollBar.active = true;
                    scrollBar.handle.width = 10;
                }
            }
        }

        Component {
            id: numberDelegate
            Rectangle {
                id:item_warp
                width: ListView.view.width
                height: 30
                color: ListView.isCurrentItem?"#CCE8FF":"#ffffff" //选中颜色设置
                border.color:ListView.isCurrentItem? "#99D1FF":"#ffffff"
                Row {
                    spacing: 2
                    Row{
                        width:30
                        height:30
                        Image {
                            id: name
                            source: type=="文件夹"?"dir.jpg":"file.jpg"
                            width:20
                            height:20
                            anchors.centerIn: parent
                        }
                    }

                    Text {
                        width:400
                        height:30
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 15
                        text: showName
                    }

                    Text {
                        width:100
                        height:30
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 15
                        text: type
                    }
                    Text {
                        width:100
                        height:30
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 15
                        text: size
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked:{
                        acceptedButtons: Qt.LeftButton // 激活左键
                        m_listView.currentIndex = index  //实现item切换
                        ctl.cppSlot(index)
                    }
                }
                MouseArea {
                    id: mouseRegion
                    anchors.fill: parent;
                    acceptedButtons:Qt.RightButton // 激活右键

                    onClicked: {
                        if (mouse.button === Qt.RightButton) { // 右键菜单
                            m_listView.currentIndex = index  //实现item切换
                            acceptedButtons: Qt.LeftButton // 激活左键
                            ctl.cppSlot(index)
                            contentMenu.popup()
                        }
                    }
                }

            }
        }
    }

    Menu { // 右键菜单
        //title: "Edit"
        id: contentMenu

        MenuItem {
            text: "剪切"
            shortcut: "Ctrl+X"
            onTriggered: {
                ctl.cppMethod("剪切")
            }
        }

        MenuItem {
            text: "复制"
            shortcut: "Ctrl+C"
            onTriggered: {
                //            m_listView.model.r
            }
        }

        MenuItem {
            text: "粘贴"
            shortcut: "Ctrl+V"
            onTriggered: {}
        }

        MenuItem {
            text: "解密"
            onTriggered: {}
        }

        MenuSeparator { }

        Menu {
            title: "More Stuff"

            MenuItem {
                text: "Do Nothing"
            }
        }
    }

    function changePwdGui(msg){
        pwdGui.text=msg
    }


    function changeCurrPwdGui(){
        pwdGui.text=ctl.getpwd()
    }
}
