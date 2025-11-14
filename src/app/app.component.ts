import { Component, EventEmitter, Input, OnInit, Output } from '@angular/core';
import { NgxSerial } from 'ngx-serial';
import { Subject } from 'rxjs';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent {
  title = 'ngx-serial-example';

  serial: NgxSerial;
  port: any;
  @Input() inputData: string = "0";
  @Output() outputData: string = "";
  onMessage = new Subject<string>();

  constructor() {
    console.info("AppComponent initialized.");
    this.serial = new NgxSerial(this.dataHandler);
    this.onMessage.subscribe((msg) => {
      console.log("Message received in component: " + msg);
    });
    localStorage.clear();
    this.updateUIFromLocalStorage();
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

  toggleL1() {
    if (this.port)
      this.serial.sendData("100\n");
  }

  toggleL2() {
    if (this.port)
      this.serial.sendData(this.inputData + "\n");
  }

  motorUp() {
    if (this.port)
      this.serial.sendData("1\n");
  }

  motorDown() {
    if (this.port)
      this.serial.sendData("2\n");
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