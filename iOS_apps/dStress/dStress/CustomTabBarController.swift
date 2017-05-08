//
//  CustomTabBarController.swift
//  dStress
//
//  Created by Ahmed Musse on 4/16/17.
//  Copyright © 2017 RAN. All rights reserved.
//

import Foundation

class CustomTabBarController: UITabBarController {
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
    var boDevices: [Any]! = [Any]()
    var device: EmpaticaDeviceManager!
    var bp5Controller: BP5Controller!
    
    // Timers
    var UITimer: Timer!
    var DataTimer: Timer!
    var BPMTimer: Timer!
    var SessionTimer: Timer!
    var startTime: TimeInterval!
    
    // Data to be sent to cloud
    var data = [String: [Any]]()
    
    var po3isConnected = false
    var bp5isConnected = false
    var empisConnected = false
    
    override func viewDidLoad() {
        super.viewDidLoad()
//        self.data = [String: [Any]]()
//        self.boDevices = [Any]()
//        self.device = nil
 
    }

    
}
