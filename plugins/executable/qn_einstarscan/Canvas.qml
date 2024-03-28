import QtQuick 2.12
import QtQuick.Controls 2.2
import Sn3D.RenderKit 1.0

Rectangle{
    width: 1200
    height: 600
    //color: "green"


	Canvas3D{
        id: canvas00
		width:600
		height: 600
		anchors.right: parent.right
        //anchors.fill: parent
        clearColor: Qt.rgba(0.0,0.0,0.0,0.0)
        maxFrame: 60
        onRendererCreated: $hub.setCanvas(canvas00);
	
        //Rectangle {
        //    id: captureRect
        //    visible: false
        //    x: 700
        //    y: 200
        //    width: 480
        //    height: 320
        //    color: "transparent"
        //    border.color: "#8FFF0000"
        //    border.width: 2
        //}
    }
	
	//Button{
    //    width: 100
    //    height: 30
	//
    //    //anchors.centerIn: parent
	//
	//	text: "IncreaseUpdate"
    //    onClicked: {
    //        $hub.startPointIncreUpdate()
	//	}
    //        
    //}
}
