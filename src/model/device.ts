export interface DeviceStatus {
  Status: {
    targetHeight: number;
    currentHeight: number;
    boxIsOn: number;
    endschalterErreicht: number;
    MAX_DISTANCE: number;
  }
}

const defaultDeviceStatus: DeviceStatus = {Status: { targetHeight: 0.00, currentHeight: -999.00, boxIsOn: 1, endschalterErreicht: 1, MAX_DISTANCE: 208.00 }};