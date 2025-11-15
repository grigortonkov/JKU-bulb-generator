import { Component, Input, OnInit } from '@angular/core';

@Component({
  selector: 'app-device',
  templateUrl: './device.component.html',
  styleUrls: ['./device.component.scss']
})
export class DeviceComponent implements OnInit {

  @Input() position: number = 10; //Abstand Deckel in mm
  constructor() { }

  ngOnInit(): void {
  }

}
