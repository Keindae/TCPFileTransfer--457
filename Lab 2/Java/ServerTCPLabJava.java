import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;


class tcpserver{
	public static void main(String args[]){
		try{
			ServerSocketChannel c = ServerSocketChannel.open();
			c.bind(new InetSocketAddress(9444));
			while(true){
				SocketChannel sc = c.accept();
				ByteBuffer buffer = ByteBuffer.allocate(4096);
				sc.read(buffer);
				String message = new String(buffer.array());
				System.out.println(message);
				sc.close();
			}
		} catch(IOException e){
			System.out.println("Got an IO Exception");
		}
	}
}
