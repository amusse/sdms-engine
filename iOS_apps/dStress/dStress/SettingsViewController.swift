//
//  SettingsViewController.swift
//  dStress
//
//  Created by Ahmed Musse on 4/16/17.
//  Copyright © 2017 RAN. All rights reserved.
//

import Foundation
import UIKit

class SettingsViewController: UIViewController  {

    @IBOutlet weak var lEmpBattery: UILabel! = UILabel()
    @IBOutlet weak var lStatus: UILabel! = UILabel()

    
    @IBOutlet weak var lBOStatus: UILabel! = UILabel()

    @IBOutlet weak var lBOBattery: UILabel! = UILabel()

    
    @IBOutlet weak var lBPMStatus: UILabel! = UILabel()

    @IBOutlet weak var lBPMBattery: UILabel! = UILabel()
    
    override func viewDidLoad() {
        let tb = self.tabBarController as! CustomTabBarController
        if tb.po3isConnected {
            lBOStatus.text = "Connected"
            lBOStatus.textColor = .green
        }
        if tb.bp5isConnected {
            lBPMStatus.text = "Connected"
            lBPMStatus.textColor = .green
        }
        if tb.empisConnected {
            lStatus.text = "Connected"
            lStatus.textColor = .green
        }
        
    }
    
    override func viewDidAppear(_ animated: Bool) {
        let tb = self.tabBarController as! CustomTabBarController
        if tb.po3isConnected {
            lBOStatus.text = "Connected"
            lBOStatus.textColor = .green
        }
        if tb.bp5isConnected {
            lBPMStatus.text = "Connected"
            lBPMStatus.textColor = .green
        }
        if tb.empisConnected {
            lStatus.text = "Connected"
            lStatus.textColor = .green
        }
    }

}
