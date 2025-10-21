package utils.processor;

import machine.Client;
import utils.channel.Channel;
import utils.channel.ChannelBasic;
import utils.message.*;

import java.io.IOException;
import java.net.ServerSocket;

public class ClientProcessor implements Processor{
    private Client client = null;

    public void setCLient(Client client){
        this.client = client;
    }

    @Override
    public Message process(Channel channel, String variableId) throws IOException, ClassNotFoundException {
        Message message = (Message) channel.recv();

        if (message instanceof ServerMessage) {
            ServerMessage serverMessage = (ServerMessage) message;
            if (serverMessage.getMessageType() == MessageType.DAR) {

                Channel distanceChannel = client.connectToClient(serverMessage.getClientHost(), serverMessage.getClientPort());

                SendDataMessage sendDataMessage = new SendDataMessage(variableId, client.getHost(), client.getPort());

                distanceChannel.send(sendDataMessage);

                SendDataMessage replyMessage = (SendDataMessage) distanceChannel.recv();

                client.modifyHeap(sendDataMessage.getVariableId(), replyMessage.getValue());
            }
        }else if (message instanceof HeartbeatMessage){
            System.out.println("砰");
            HeartbeatMessage heartbeatMessage = (HeartbeatMessage) message;
            if (heartbeatMessage.getOp() == OperationStatus.HEART){
                System.out.println("砰砰");
                HeartbeatMessage hbm = new HeartbeatMessage(MessageType.HBM,OperationStatus.HEARTNORMAL);
                channel.send(hbm);
            }
            System.out.println("砰砰砰");
        }

        return message;
    }

}
