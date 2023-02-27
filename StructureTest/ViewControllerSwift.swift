//
//  ViewController.swift
//  StructureTest
//
//  Created by Sebas Bejenaru on 27/2/23.
//

import UIKit
import ScannerFramework

class ViewControllerSwift: UIViewController {

    @IBAction func redirect(_ sender: Any) {
        
        let scannerController = CompanyNow()
        present(scannerController, animated: true)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
       
        
    }


}

