export interface DeviceStatus {
  Status: {
    targetHeight: number;
    currentHeight: number;
    boxIsOn: number;
    endschalterErreicht: number;
  }
}

const defaultDeviceStatus: DeviceStatus = {Status: { targetHeight: 0.00, currentHeight: -999.00, boxIsOn: 1, endschalterErreicht: 1 }};