class Bridge:

    def __init__(self, bridgeID, noOfPorts, portID, portIDLans):
        self.bridgeID = bridgeID
        self.noOfPorts = noOfPorts
        self.portID = portID
        self.portIDLans = portIDLans

        self.rootBID = bridgeID
        self.isRootB = 1
        self.minRootPathB = bridgeID
        self.minRootPathPort = ""
        self.distToRootB = 0
        self.typeOfPorts = self.defaultDPs(bridgeID, noOfPorts)
        self.trueDP = self.trueDPs(bridgeID, noOfPorts)

        self.time = 0
        self.firstTime = 1

    def defaultDPs(self, bridgeID, noOfPorts):  # every port is assigned DP at the start
        typeOfPorts = dict()
        for z in range(0, noOfPorts):
            typeOfPorts.setdefault('P'+str(bridgeID.replace('B', '')
                                           ) + str(z+1), "DP")
        return typeOfPorts

    def trueDPs(self, bridgeID, noOfPorts):  # for keeping check of true DPs
        trueDP = dict()
        for z in range(0, noOfPorts):
            trueDP.setdefault('P'+str(bridgeID.replace('B', '')
                                      ) + str(z+1), 1)
        return trueDP

    def sendInitMsg(self, time, portID, outTrace, traceReq):
        msg = [time, "s", self.bridgeID, self.rootBID,
               self.distToRootB, self.bridgeID, portID]
        traceMsg = (
            f"{msg[0]} s {msg[2]} ({msg[3]},{msg[4]},{msg[5]}) ||| Initialization, send on {msg[6]} ||| {self.bridgeID} is root, all ports DP\n")
        if traceReq == 1:
            outTrace.write(traceMsg)
        return msg

    def sendMsg(self, time, portID, outTrace, traceReq):
        msg = [time, "s", self.bridgeID, self.rootBID,
               self.distToRootB, self.bridgeID, portID]
        if self.isRootB == 1:
            traceMsg = (
                f"{msg[0]} s {msg[2]} ({msg[3]},{msg[4]},{msg[5]}) ||| {self.bridgeID} still doesn't have a 'better message' ||| {self.bridgeID} is root\n")
            if traceReq == 1:
                outTrace.write(traceMsg)
            return msg
        if self.isRootB == 0:
            if self.typeOfPorts[portID] == "DP" or self.typeOfPorts[portID] == "RP":
                traceMsg = (
                    f"{msg[0]} s {msg[2]} ({msg[3]},{msg[4]},{msg[5]}) ||| Message send on port {msg[6]} ||| {self.rootBID} is root, this port {self.typeOfPorts[portID]}\n")
                if traceReq == 1:
                    outTrace.write(traceMsg)
                return msg

    def receiveMsg(self, time, portID, msgs, outTrace, traceReq):
        for x in msgs:
            if x[2] != self.bridgeID:
                x3temp = int(x[3].replace('B', ''))
                rBIDtemp = int((self.rootBID).replace('B', ''))
                # for detecting root bridge and RP
                if x3temp <= rBIDtemp and x[3] != self.bridgeID:
                    tempRB = self.rootBID
                    self.rootBID = x[3]
                    self.isRootB = 0
                    if self.firstTime == 1:
                        self.minRootPathB = x[5]
                        self.minRootPathPort = x[6]
                        self.firstTime = 0
                    if self.distToRootB != 0:
                        if tempRB != self.rootBID:
                            self.distToRootB = x[4] + 1
                            for y in self.typeOfPorts:
                                if self.typeOfPorts[y] == "RP":
                                    self.typeOfPorts[y] = "DP"
                            self.typeOfPorts[portID] = "RP"
                            self.minRootPathB = x[5]
                            self.minRootPathPort = x[6]
                        else:
                            if (x[4]+1) < self.distToRootB:
                                self.distToRootB = x[4] + 1
                                for y in self.typeOfPorts:
                                    if self.typeOfPorts[y] == "RP":
                                        self.typeOfPorts[y] = "DP"
                                self.typeOfPorts[portID] = "RP"
                            elif (x[4]+1) == self.distToRootB:
                                x5temp = int(x[5].replace('B', ''))
                                minRPBtemp = int(
                                    (self.minRootPathB).replace('B', ''))
                                if x5temp < minRPBtemp:
                                    for y in self.typeOfPorts:
                                        if self.typeOfPorts[y] == "RP":
                                            self.typeOfPorts[y] = "DP"
                                    self.typeOfPorts[portID] = "RP"
                                    self.minRootPathB = x[5]
                                elif x[5] == self.minRootPathB:
                                    x6temp = int(x[6].replace('P', ''))
                                    minRPPtemp = int(
                                        (self.minRootPathPort).replace('P', ''))
                                    if x6temp < minRPPtemp:
                                        for y in self.typeOfPorts:
                                            if self.typeOfPorts[y] == "RP":
                                                self.typeOfPorts[y] = "DP"
                                        self.typeOfPorts[portID] = "RP"
                                        self.minRootPathPort = x[6]
                    elif self.distToRootB == 0:
                        self.distToRootB = x[4] + 1
                        self.typeOfPorts[portID] = "RP"
                    if self.typeOfPorts[portID] == "RP":
                        traceMsg = (
                            f"{time} r {self.bridgeID} ({x[3]},{x[4]},{x[2]}) ||| Message reaching port {portID} ||| {self.rootBID} is root, this port RP\n")
                    else:
                        traceMsg = (
                            f"{time} r {self.bridgeID} ({x[3]},{x[4]},{x[2]}) ||| Message reaching port {portID} ||| {self.rootBID} is root, this port DP\n")
                else:
                    traceMsg = (
                        f"{time} r {self.bridgeID} ({x[3]},{x[4]},{x[2]}) ||| Message reaching port {portID} ||| {self.rootBID} is root, all RB ports DP\n")
                if traceReq == 1:
                    outTrace.write(traceMsg)

            if ((x[2] != self.bridgeID) and (self.isRootB == 0)):  # for detecting NPs
                if ((self.typeOfPorts[portID] != "RP") and (self.rootBID == x[3])):
                    if self.distToRootB > x[4]:
                        self.trueDP[portID] = 0
                    elif self.distToRootB == x[4]:
                        x5temp = int(x[5].replace('B', ''))
                        BIDtemp = int((self.bridgeID).replace('B', ''))
                        if BIDtemp > x5temp:
                            self.trueDP[portID] = 0
                        elif self.bridgeID == x[5]:
                            x6temp = int(x[6].replace('B', ''))
                            PIDtemp = int(portID.replace('B', ''))
                            if PIDtemp > x6temp:
                                self.trueDP[portID] = 0

    def finalConfig(self):  # for printing final output
        for z in self.typeOfPorts:
            if self.bridgeID != self.rootBID:
                if self.typeOfPorts[z] == "DP":
                    if self.trueDP[z] == 0:
                        self.typeOfPorts[z] = "NP"

        if "DP" not in (self.typeOfPorts).values():
            for z in self.typeOfPorts:
                self.typeOfPorts[z] = "NP"
        output = (str(self.bridgeID)+':')
        for portID, LANName in self.portIDLans.items():
            output = output + (' '+str(LANName)+"-" +
                               str(self.typeOfPorts[portID]))
        print(output)


class LAN:  # intermediate between the bridges

    def __init__(self, LANName):
        self.LANName = LANName
        self.msgs = []

    def storeMsg(self, msg):
        (self.msgs).append(msg)

    def giveMsg(self):
        return self.msgs

    def clearMsg(self):
        (self.msgs).clear()
