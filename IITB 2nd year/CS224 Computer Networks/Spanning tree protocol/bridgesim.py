import sys
from bridge import Bridge, LAN

traceReq = bool(int(input()))
noOfBridges = int(input())

lines = []
for x in range(0, noOfBridges):
    lines.append(input())

if traceReq == 1:
    outFile2 = sys.argv[2]
    outTrace = open(outFile2, 'w')
elif traceReq == 0:
    outTrace = None

bridges = []
LANNames = []
for x in range(1, noOfBridges + 1):  # initializing the bridges
    bridgeID = 'B' + str(int((lines[x-1].split('B'))[1].split(':')[0]))
    portIDNames = lines[x-1].split(':', 1)[1]
    noOfPorts = (len(portIDNames) - portIDNames.count(' ')) - \
        portIDNames.count("\n")
    portIDNamesList = sorted(portIDNames.split())
    for y in range(0, len(portIDNamesList)):
        if portIDNamesList[y] not in LANNames:
            LANNames.append(portIDNamesList[y])

    portID = []
    for y in range(0, len(portIDNamesList)):
        portID.append('P'+str(x)+str(y+1))

    portIDLans = dict()
    for y in range(0, len(portIDNamesList)):
        portIDLans['P'+str(x)+str(y+1)] = portIDNamesList[y]
    bridges.append(Bridge(bridgeID, noOfPorts, portID, portIDLans))

LANNames.sort()
LANs = dict()
for x in LANNames:  # initializing the LANs
    LANs[x] = LAN(x)

time = 0  # start simulation
for B in bridges:
    for P in B.portID:
        LANs[B.portIDLans[P]].storeMsg(
            B.sendInitMsg(time, P, outTrace, traceReq))

while True:
    BIDs = []
    noOfDPs = 0
    for B in bridges:
        BIDs.append(B.rootBID)
        for y in B.trueDP:
            if ((B.trueDP[y] == 1) and (B.typeOfPorts[y] != "RP")):
                noOfDPs += 1
    if ((not (all(x == BIDs[0] for x in BIDs))) or (not (len(LANNames) == noOfDPs))):
        time = time + 1
        for B in bridges:
            for P in B.portID:
                for x in B.portIDLans[P]:
                    msgs = LANs[B.portIDLans[P]].giveMsg()
                    if len(msgs) > 0:
                        B.receiveMsg(time, P, msgs, outTrace, traceReq)
        for x in LANs:
            LANs[x].clearMsg()

        for B in bridges:
            for P in B.portID:
                if B.typeOfPorts[P] != "NP":
                    LANs[B.portIDLans[P]].storeMsg(
                        B.sendMsg(time, P, outTrace, traceReq))
        continue
    else:
        break

for B in bridges:  # final output
    B.finalConfig()
