//
//  ViewController.swift
//  dStress
//
//  Created by Ahmed Musse on 1/25/17.
//  Copyright © 2017 RAN. All rights reserved.
//

import UIKit
import Alamofire
import AVFoundation

class DataViewController: UIViewController, EmpaticaDelegate, EmpaticaDeviceDelegate {
    
    // Empatica Labels
    @IBOutlet weak var lBVP: UILabel!
    @IBOutlet weak var lGSR: UILabel!
    @IBOutlet weak var lTemp: UILabel!
    @IBOutlet weak var lIBI: UILabel!
    
    @IBOutlet weak var lEmpBattery: UILabel!
    @IBOutlet weak var btnOutDisconnect: UIButton!
    @IBOutlet weak var lStatus: UILabel!
    
    // Pulse Oximeter Labels
    @IBOutlet weak var lBOStatus: UILabel!
    @IBOutlet weak var lBO: UILabel!
    @IBOutlet weak var lBOBattery: UILabel!
    
    // BPM Labels
    @IBOutlet weak var lDiaBP: UILabel!
    @IBOutlet weak var lSysBP: UILabel!
    @IBOutlet weak var lBPMStatus: UILabel!
    @IBOutlet weak var lBPMBattery: UILabel!
    
    @IBOutlet weak var lDataStatus: UILabel!
    
    @IBOutlet weak var lineChart: LineChart!
    
    // Empatica Features
    var BVP: String!
    var GSR: String!
    var IBI: String!
    var temp: String!
    var accX: String!
    var accY: String!
    var accZ: String!
    var empBatteryLevel: String!
    
    // Pulse Oximeter Features
    var BO: String!
    
    // BPM Features
    var diaBP: String!
    var sysBP: String!
    
    // Device Instances
    var boDevices: [Any]!
    var device: EmpaticaDeviceManager!
    var bp5Controller: BP5Controller!
    
    // Timers
    var UITimer: Timer!
    var DataTimer: Timer!
    var BPMTimer: Timer!
    var SessionTimer: Timer!
    var startTime: TimeInterval!
    // Data to be sent to cloud
    var data: [String: [Any]]!
    
    var numDevicesConnected = 0
    var didDiscoverEmpatica = false
    
    var settingsVC: SettingsViewController!
    
    var xStartTime = 0.0
    var count = 0
    var clearedBVP = false
    
    
    // ---------------- Begin Functions ------------------------//
    
    // Updates the UI every 0.5 seconds with sensor readings
    func updateUI() -> Void {
//        self.lEmpBattery.text = self.empBatteryLevel
        self.lBVP.text = self.BVP
        self.lGSR.text = self.GSR
        self.lIBI.text = self.IBI
        self.lTemp.text = self.temp
        self.lBO.text = self.BO
        self.lDiaBP.text = self.diaBP
        self.lSysBP.text = self.sysBP
        
        if self.numDevicesConnected == 2 {
            self.numDevicesConnected = 0
            print("Beginning to send data")
            // Start Data timer
//            self.DataTimer = Timer.scheduledTimer(timeInterval: FOUR_MINUTES, target: self, selector: #selector(self.sendData), userInfo: nil, repeats: true)
            
            
            // Start BPM timer
//            getBP()
//            self.BPMTimer = Timer.scheduledTimer(timeInterval: THREE_MINUTES, target: self, selector: #selector(self.getBP), userInfo: nil, repeats: true)
        }

        
        let f: ([Any]) -> CGPoint = {point in
            let x = Double(point[0] as! Double) - self.xStartTime
            let y = point[1] as! Double
            //            print(y)
            return CGPoint(x: x, y: y)
        }
        
        var xs = [[Any]]()
        var bvps = [Double]()
        
        if let _ = self.data["BVP"] {
            var data = self.data["BVP"] as! [[Any]]
//            print(data)
            for sample in data {
                bvps.append(Double(sample[1] as! Float))
            }
            let maxVal = bvps.max()
            let minVal = bvps.min()
            var index = 0
            for _ in data {
                data[index][1] = (Double(data[index][1] as! Float) - minVal!)/(maxVal! - minVal!)
                index = index + 1
            }
            xs = data
        }
        
        let points = xs.map({f($0)})
        
        lineChart.deltaX = 20
        lineChart.deltaY = 30
        
        lineChart.plot(points)
        self.count = self.count + 1
        
        
    }
    
    func findEmpatica() {
        if !self.didDiscoverEmpatica {
            EmpaticaAPI.discoverDevices(with: self)
        }
    }

    
    // Sends collected data to cloud server
    func sendData() -> Void {
        print("Sending Data")
        self.lDataStatus.text = "Sending Data..."
        self.lDataStatus.textColor = .blue
        print(self.data)

        Alamofire.request(CLASSIFY_API, method: .post, parameters: self.data, encoding: JSONEncoding.default).responseJSON { (response) in
            // delete data
            let resp = response.result.description.lowercased()
            print("Server Response: " + resp)
            if resp == "success" {
                print("Data sent")
                self.lDataStatus.text = "Data Sent"
                self.lDataStatus.textColor = .green
            } else {
                print("Data not sent")
                self.lDataStatus.text = "Data Not Sent"
                self.lDataStatus.textColor = .red
            }
            
            self.data = [String: [Any]]()
        }
    }
    
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
//        btnOutDisconnect.isHidden = true
        self.data = [String: [Any]]()
        self.boDevices = [Any]()
        let tb = self.tabBarController as! CustomTabBarController
        self.device = tb.device
        
        NotificationCenter.default.addObserver(self, selector: #selector(self.po3DidConnect), name: NSNotification.Name(rawValue: PO3ConnectNoti), object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(self.bp5DidConnect), name: NSNotification.Name(rawValue: BP5ConnectNoti), object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(self.po3DidDisconnect), name: NSNotification.Name(rawValue: PO3DisConnectNoti), object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(self.bp5DidDisconnect), name: NSNotification.Name(rawValue: BP5DisConnectNoti), object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(self.po3DidDiscover), name: NSNotification.Name(rawValue: PO3Discover), object: nil)
        NotificationCenter.default.addObserver(self, selector: #selector(self.po3DidFailConnect), name: NSNotification.Name(rawValue: PO3ConnectFailed), object: nil)
        self.bp5Controller = BP5Controller.share()
        
        // Every half-second update the UI
        self.UITimer = Timer.scheduledTimer(timeInterval: 0.1, target: self, selector: #selector(self.updateUI), userInfo: nil, repeats: true)
        self.UITimer = Timer.scheduledTimer(timeInterval: 1, target: self, selector: #selector(self.findEmpatica), userInfo: nil, repeats: true)
        
        EmpaticaAPI.discoverDevices(with: self)
        ScanDeviceController.commandGetInstance().commandScanDeviceType(HealthDeviceType_PO3)

        self.settingsVC = self.tabBarController?.viewControllers?[1] as! SettingsViewController
    }

    
    // ---------------- Blood Pressure Monitor Functions ------------------------//
    
    @IBAction func btnMeasureBP(_ sender: UIButton) {
        switch sender.tag {
        case 0:
            print("Beginning BP Measurement")
            getBP()
            //Variables
            break
        case 1:
            let bp5array = self.bp5Controller.getAllCurrentBP5Instace()
            if (bp5array?.count)! > 0 {
                let bp5 = bp5array?[0] as! BP5
                let user = HealthUser()
                user.clientID = CLIENT_ID
                user.clientSecret = CLIENT_SECRET
                user.userID = USER_ID
                
                bp5.commandEnergy({ (battLevel) in
                    self.lBPMBattery.text = battLevel?.description
                }, errorBlock: { (err) in
                    print("ErrorID: " + err.rawValue.description)
                })
            }
            break
        default:
            break
        }
        
    }
    func bp5DidConnect(info: Notification) -> Void {
        let bp5array = self.bp5Controller.getAllCurrentBP5Instace()
        print("Connected device: " + (info.userInfo?.description)!)
        if (bp5array?.count)! > 0 {
            print("Connected Success")
            self.numDevicesConnected += 1
            let tb = self.tabBarController as! CustomTabBarController
            tb.bp5isConnected = true
        }
    }
    
    func bp5DidDisconnect(info: Notification) -> Void {
        print("Device disconnected: " + info.description)
        let tb = self.tabBarController as! CustomTabBarController
        tb.bp5isConnected = false
    }
    
    func getBP() {
        let bp5array = self.bp5Controller.getAllCurrentBP5Instace()
        if (bp5array?.count)! > 0 {
            let bp5 = bp5array?[0] as! BP5
            let user = HealthUser()
            user.clientID = CLIENT_ID
            user.clientSecret = CLIENT_SECRET
            user.userID = USER_ID
        
            bp5.commandStartMeasure(withUser: user.userID, clientID: user.clientID, clientSecret: user.clientSecret, authentication: { (auth) in
                if auth.rawValue != 2 {
                    print("Authorization Failed!")
                }
            }, pressure: { (pressures) in
            }, xiaoboWithHeart: { (xwh) in
            }, xiaoboNoHeart: { (xnh) in
            }, result: { (results) in
                let bpmResults = results! as [AnyHashable: Any]
                let sys = bpmResults["sys"] as! Int
                let dia = bpmResults["dia"] as! Int
                let hr = bpmResults["heartRate"] as! Int
                let irreg = bpmResults["irregular"] as! Int
                
                self.sysBP = sys.description
                self.diaBP = dia.description
                self.lSysBP.text = self.sysBP
                self.lDiaBP.text = self.diaBP
                
                let timestamp = NSDate().timeIntervalSince1970
                let data = [timestamp.description, sys, dia, hr, irreg] as [Any]
                if var bpData = self.data["BP"] {
                    bpData.append(data)
                    self.data["BP"] = bpData
                } else {
                    self.data["BP"] = [data]
                }
            }, errorBlock: { (error: BPDeviceError) in
                print("ErrorID: " + error.rawValue.description)
            })
        }

        
    }
    
    // ---------------- Pulse Oximeter Functions ------------------------//
    
    @IBAction func btnBOControls(_ sender: UIButton) {
        let po3controller = PO3Controller.shareIHPO() as PO3Controller
        let po3array = po3controller.getAllCurrentPO3Instace() as [Any]
        let user = HealthUser()
        var po3: PO3!
        if po3array.count > 0 {
            po3 = po3array[0] as! PO3
            user.clientID = CLIENT_ID
            user.clientSecret = CLIENT_SECRET
            user.userID = USER_ID
        }
        
        switch sender.tag {
        case SelectedBOButtonTag.ScanBO.rawValue:
            print("Start Scan")
            
            ScanDeviceController.commandGetInstance().commandScanDeviceType(HealthDeviceType_PO3)
            break
        case SelectedBOButtonTag.DisconnectBO.rawValue:
            print("Device Disconnected")
            po3.commandDisconnect({ (resetSuc) in
                print("Disconnected Status: "  + resetSuc.description)
            }, withErrorBlock: { (errorID: PO3ErrorID) in
                print("ErrorID: " + errorID.rawValue.description)
            })
            break
        case SelectedBOButtonTag.BatteryBO.rawValue:
            po3.commandGetDeviceBattery({ (battery) in
                print("Battery percentage: " + (battery?.description)!)
                self.lBOBattery.text = battery?.description
            }, withErrorBlock: { (error: PO3ErrorID) in
                print("ErrorID: " + error.rawValue.description)
            })
            break
        default:
            break
        }
    }
    
    func po3DidConnect(info: Notification) -> Void {
        let po3controller = PO3Controller.shareIHPO()
        let po3array = po3controller?.getAllCurrentPO3Instace()
        print("Connected device: " + (info.userInfo?.description)!)
        if (po3array?.count)! > 0 {
            let po3 = po3array?[0] as! PO3
            let user = HealthUser()
            user.clientID = CLIENT_ID
            user.clientSecret = CLIENT_SECRET
            user.userID = USER_ID
            print("Connected Success")
            
//            self.lBOStatus.text = "Authenticating..."
//            self.lBOStatus.textColor = .orange
            
            print("Authenticating..")
            po3.commandCreatePO3User(user, authentication: { (result: UserAuthenResult) in
                print("User authentication: "  + result.rawValue.description)
                if result.rawValue == 2 {
                    po3.commandStartMeasureData({ (startData) in
                        print("Transmitting Data: " + startData.description)
                        self.numDevicesConnected += 1
                        let tb = self.tabBarController as! CustomTabBarController
                        tb.po3isConnected = true
                        
                    }, measure: { (measureDataDic) in
                        if let bo = measureDataDic?["spo2"] as? Int {
                            let boResults = measureDataDic! as [AnyHashable: Any]
                            self.BO = bo.description
//                            self.lBO.text = self.BO
                            
                            let bpm = boResults["bpm"] as! Int
                            let pi = boResults["PI"] as! Double
                            
                            let timestamp = NSDate().timeIntervalSince1970
                            let data = [timestamp.description, bo, pi, bpm] as [Any]
                            if var boData = self.data["BO"] {
                                boData.append(data)
                                self.data["BO"] = boData
                            } else {
                                self.data["BO"] = [data]
                            }
                        }
                    }, finishPO3MeasureData: { (finishData) in
                        print("Transmission Ended: " + finishData.description)
                    }, disposeErrorBlock: { (errorID: PO3ErrorID) in
                        print("ErrorID: " + errorID.rawValue.description)
                    })
                }
            }, disposeResultBlock: { (finishSynchronous) in
                let tb = self.tabBarController as! CustomTabBarController
                tb.po3isConnected = true
            }) { (errorID: PO3ErrorID) in
                print("Error: " + errorID.rawValue.description)
            }
        }
        
    }

    func po3DidDisconnect(info: Notification) -> Void {
        print("Device disconnected: " + info.description)
        let tb = self.tabBarController as! CustomTabBarController
        tb.po3isConnected = false
    }
    
    func po3DidDiscover(info: Notification ) -> Void {
        print("Discovered: " + info.description)
        self.boDevices.append(info.userInfo!)
        let serialNum = info.userInfo?["SerialNumber"]
        let id = info.userInfo?["ID"]
 
        if serialNum != nil {
            ConnectDeviceController.commandGetInstance().commandContectDevice(with: HealthDeviceType_PO3, andSerialNub: serialNum as! String)
        } else {
            ConnectDeviceController.commandGetInstance().commandContectDevice(with: HealthDeviceType_PO3, andSerialNub: id as! String)
        }
        ScanDeviceController.commandGetInstance().commandStopScanDeviceType(HealthDeviceType_PO3)
    }
    
    func po3DidFailConnect(info: Notification) -> Void {
        print("Connection failed: " + info.description)
//        self.lBOStatus.text = "Connection Falied"
//        self.lBOStatus.textColor = .red
    }

    // ---------------- Empatica Functions ------------------------//
    
    func didDiscoverDevices(_ devices: [Any]!) {
        if (devices.count > 0) {
            // Connecting to first available device
            let device = devices[0] as! EmpaticaDeviceManager
            let tb = self.tabBarController as! CustomTabBarController
            tb.device = device
            self.device = device
            device.connect(with: self)
        } else {
            print("No device found in range")
//            lStatus.text = "No Devices Found"
//            lStatus.textColor = .red
        }
    }
    
    func didUpdate(_ status: BLEStatus) {
        switch(status.rawValue) {
        case kBLEStatusNotAvailable.rawValue:
            //            print("BLE is not available")
            break
        case kBLEStatusReady.rawValue:
            //            print("BLE is on, scanning")
            break
        case kBLEStatusScanning.rawValue:
            //            print("Scanning BLE")
//            lStatus.text = "Scanning..."
//            lStatus.textColor = .blue
            break
        default:
            print("Default state")
            break
        }
    }
    
    func didUpdate(_ status: DeviceStatus, forDevice device: EmpaticaDeviceManager!) {
        switch(status.rawValue) {
        case kDeviceStatusDisconnected.rawValue:
            print("Device has disconnected")
//            btnOutDisconnect.isHidden = true
            let tb = self.tabBarController as! CustomTabBarController
            tb.empisConnected = false
            break
        case kDeviceStatusConnecting.rawValue:
            print("Device is connecting: " + device.description)
            break
        case kDeviceStatusConnected.rawValue:
            print("Device is connected")
            self.numDevicesConnected += 1
            self.didDiscoverEmpatica = true
            let tb = self.tabBarController as! CustomTabBarController
            tb.empisConnected = true
//            btnOutDisconnect.isHidden = false
            break
        case kDeviceStatusFailedToConnect.rawValue:
            print("Device failed to connect")
//            lStatus.text = "Failed to connect"
//            lStatus.textColor = .purple
            break
        case kDeviceStatusDisconnecting.rawValue:
            print("Device is disconnecting")
//            lStatus.text = "Disconnecting..."
//            lStatus.textColor = .orange
            break
        default:
            print("Default state")
            break
        }
    }
    func didReceiveTag(atTimestamp timestamp: Double, fromDevice device: EmpaticaDeviceManager!) {
        print("Received tag at " + timestamp.description)
        
    }
    
    func didReceiveGSR(_ val: Float, withTimestamp timestamp: Double, fromDevice device: EmpaticaDeviceManager!) {
        self.GSR = val.description
        let data = [timestamp, val] as [Any]
        if var gsr = self.data["GSR"] {
            gsr.append(data)
            self.data["GSR"] = gsr
        } else {
            self.data["GSR"] = [data]
        }
    }
    
    func didReceiveIBI(_ val: Float, withTimestamp timestamp: Double, fromDevice device: EmpaticaDeviceManager!) {
        self.IBI = val.description
        let data = [timestamp, val] as [Any]
        if var ibi = self.data["IBI"] {
            ibi.append(data)
            self.data["IBI"] = ibi
        } else {
            self.data["IBI"] = [data]
        }
        
    }
    
    func didReceiveBVP(_ val: Float, withTimestamp timestamp: Double, fromDevice device: EmpaticaDeviceManager!) {
        
        if self.xStartTime == 0.0 {
            self.xStartTime = timestamp
        }
        self.BVP = val.description
        let data = [timestamp, val] as [Any]
        if var bvp = self.data["BVP"] {
            if self.count > 30 {
                bvp.append(data)
                self.data["BVP"] = bvp
            } else {
                self.data["BVP"] = [data]
            }
        
        } else {
            self.data["BVP"] = [data]
        }
        
    }
    
    func didReceiveTemperature(_ val: Float, withTimestamp timestamp: Double, fromDevice device: EmpaticaDeviceManager!) {
        self.temp = val.description
        let data = [timestamp, val] as [Any]
        if var temp = self.data["TEMP"] {
            temp.append(data)
            self.data["TEMP"] = temp
        } else {
            self.data["TEMP"] = [data]
        }
    }
    
//    func didReceiveAccelerationX(_ x: Int8, y: Int8, z: Int8, withTimestamp timestamp: Double, fromDevice device: EmpaticaDeviceManager!) {
//        self.accX = x.description
//        self.accY = y.description
//        self.accZ = z.description
//        
//    }
    
//    func didReceiveBatteryLevel(_ level: Float, withTimestamp timestamp: Double, fromDevice device: EmpaticaDeviceManager!) {
//        let batt = level*100
//        self.empBatteryLevel = batt.description
//    }
    
}
