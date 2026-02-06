import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';
import { NgxSerial } from 'ngx-serial';
import { Subject } from 'rxjs';
import { DeviceStatus } from 'src/model/device';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})

export class AppComponent {

  title = 'jku-loop-reactor';

  serial: NgxSerial;
  port: any;
  @Input() inputData: number = 0;
  @Output() outputData: string = "";
  onMessage = new Subject<string>();
  deviceStatus = new Subject<DeviceStatus>();

  currentHeight = 0;
  MAX_DISTANCE = 208;

  constructor() {
    console.info("AppComponent initialized.");
    this.serial = new NgxSerial(this.dataHandler);
    this.onMessage.subscribe((msg) => {
      console.log("Message received in component: " + msg);
      if (msg.startsWith("{Status:")) {
        const cleanedMsg = msg.replace("Status","\"Status\"")
        .replace("targetHeight","\"targetHeight\"")
        .replace("currentHeight","\"currentHeight\"")
        .replace("boxIsOn","\"boxIsOn\"")
        .replace("endschalterErreicht","\"endschalterErreicht\"")
        .replace("MAX_DISTANCE","\"MAX_DISTANCE\"");
        this.deviceStatus.next(JSON.parse(cleanedMsg) as DeviceStatus);
      }
    });
    localStorage.clear();
    this.updateUIFromLocalStorage();
    this.deviceStatus.subscribe((res) => {
      console.log("Device Status updated:", res);
      this.currentHeight = res.Status.currentHeight;
      this.MAX_DISTANCE = res.Status.MAX_DISTANCE;
    });
  }

  t() {
     this.currentHeight =  this.currentHeight + 1;
  }

  updateUIFromLocalStorage() {
    // console.trace("Updating UI from local storage...");
    setTimeout(() => {
      const lastData = localStorage.getItem('lastData');
      if (lastData) {
        this.outputData = lastData;
        this.onMessage.next(lastData);
      }

      this.updateUIFromLocalStorage();
    }, 1000);
  }

  dataHandler(data: string) {
    setTimeout(() => {
      console.log("From arduino -> " + data);
      this.outputData = data;
      // this.onMessage.next("test");
      localStorage.setItem('lastData', data);
    }, 10);
  }

  connect() {
    this.outputData = "Connecting...";
    if (!this.port) {
      this.serial.connect((port: any) => {
        this.port = port;
      });
    }
  }

  stop() {
    if (this.port)
      this.serial.sendData("0\n");
  }

  toggleL1() {
    if (this.port)
      this.serial.sendData("100\n");
  }

  setHeight() {
    if (this.port) {
      this.currentHeight = this.inputData;
      this.serial.sendData(this.inputData + "\n");
    }
  }

  motorUp() {
    if (this.port)
      this.serial.sendData("-1\n");
  }

  motorDown() {
    if (this.port)
      this.serial.sendData("1\n");
  }

  calibrate() {
    if (this.port)
      this.serial.sendData("999\n");
  }

  close() {
    if (this.port)
      this.serial.close((port: any) => {
        this.port = port;

      });
  }
}