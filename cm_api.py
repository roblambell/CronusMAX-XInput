import hid
import time

stop_pending = False

while stop_pending is False:
    try:
        print(".", end="")  # monitor how many attempts are required

        cronusmax = hid.device()
        cronusmax.open(0x2508, 0x0001)

        print("")
        #print("Manufacturer: %s" % cronusmax.get_manufacturer_string())
        #print("Product: %s" % cronusmax.get_product_string())

        # non-blocking mode
        #cronusmax.set_nonblocking(1)

        # enter capture mode
        resp = cronusmax.write(bytes([0x07]))
        print(resp)

        exit()

        # for x in range(0, 5):
        #     print("")
        #     time.sleep(1)
        #     for y in range(0, 100):
        #         print(".", end="")
        #         d = cronusmax.read(100)
        #         if d:
        #             print(d)

        data = cronusmax.read(64, 1000)
        print(data)

        # for x in range(0, 100):
        #     data = cronusmax.read(64)
        #     print(data, end="")
            # if d:
            #     print("")
            #     print("+", end="")
            #     print(d)
            # else:
            #     print(".", end="")
            #time.sleep(0.01)

        # try writing some data to the device
        # for k in range(10):
        #     for i in [0, 1]:
        #         for j in [0, 1]:
        #             h.write([0x80, i, j])
        #             d = h.read(5)
        #             if d:
        #                 print(d)
        #             time.sleep(0.05)

        print("")

        # leave capture mode + close
        print("Closing device")
        resp = cronusmax.write(bytes([0x08]))
        print(resp)
        cronusmax.close()

        stop_pending = True

    except IOError:
        # Device not found / could not be opened
        pass
