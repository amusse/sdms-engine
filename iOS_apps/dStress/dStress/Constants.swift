//
//  Constants.swift
//  dStress
//
//  Created by Ahmed Musse on 3/30/17.
//  Copyright © 2017 RAN. All rights reserved.
//

import Foundation

enum SelectedBOButtonTag: Int {
    case ScanBO
    case ConnectBO
    case AuthenticateBO
    case MeasureBO
    case DisconnectBO
    case EmptyMemoryBO
    case BatteryBO
    case FactoryResetBO
}

// iHealth Credentials
let CLIENT_ID           = "2a8387e3f4e94407a3a767a72dfd52ea"
let CLIENT_SECRET       = "fd5e845c47944a818bc511fb7edb0a77"
let USER_ID             = "he@12.com"


// Mac Server
let TRAIN_API                 = "http://9ac4fcb1.ngrok.io/api/data"
let CLASSIFY_API                 = "http://9ac4fcb1.ngrok.io/api/classify"
// Raspberry Pi Server
//let API               = "http://b5e49b34.ngrok.io/api/data"

let ONE_MINUTE: TimeInterval        = 1 * 60
let ONE_HALF_MINUTES: TimeInterval   = 1.5 * 60
let TWO_MINUTES: TimeInterval       = 2 * 60
let THREE_MINUTES: TimeInterval     = 3 * 60
let FOUR_MINUTES: TimeInterval      = 4 * 60
let FIVE_MINUTES: TimeInterval      = 5 * 60
