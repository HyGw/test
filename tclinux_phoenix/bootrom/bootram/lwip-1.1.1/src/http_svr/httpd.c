/*
 * Test the lwIP stack port.
 *
 * This runs a web server of sorts.
 */
//#include <nds.h>
//#include <stdarg.h>
//#include <stdio.h>
//#include <stdint.h>
#include <linux/string.h>

//#include "wifi_arm9.h"

#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/pbuf.h"
#include "lwip/ip.h"
#include "lwip/udp.h"
#include "lwip/tcpip.h"
#include "lwip/stats.h"
#include "netif/etharp.h"

#define DO_WEBSERVER 1

#define IP_ADDRESS_FORMAT( ip) (ip) & 0xff, ((ip) >> 8) & 0xff,\
((ip) >> 16) & 0xff, ((ip) >> 24) & 0xff

void
ClsTop( void);
void
ClsBtm( void);
void
printtop_xy( int x, int y, const char * str);
void
printbtm(int x, int y, const char * str);
void
stats_display_pbuf(struct stats_pbuf *pbuf);
void
stats_display_proto(struct stats_proto *proto, char *name);
void
stats_display_mem(struct stats_mem *mem, char *name);

int
process_http( struct pbuf *stream, struct tcp_pcb *outlet);


static char print_buffer[2048];

/* Nasty globals:
 *
 * The interface_active flag is set true when the wifi network interface is up.
 * The wifi_netif is the interface structure used by lwip.
 */
static int interface_active;
struct netif wifi_netif;

/*
 * The current web server connection.
 */
struct web_connection {
  /* the client tcp connection */
  struct tcp_pcb *current_connection;

  /* Detect idle connection using the last seq number */
  u32_t last_rx_seqno;

  /* the rx state */
  int rx_state;

  /* save the GET http path here */
  char get_path[200];

  /* use this buffer for processing received data */
  char rx_buffer[200];
} the_web_connection;

/*
 * The state of the http receive
 */
#define WAITING_HEADER_RX_STATE 0


/*
 * Buffer lines for printing
 */
#define NUM_LINE_BUFFERS 200
#define MAX_LINE_LENGTH 32
static int num_active_lines;
static int next_line_offset;
static int line_back_count;
static char line_buffers[NUM_LINE_BUFFERS][MAX_LINE_LENGTH + 1];

static void
init_print_sys( void) {
  num_active_lines = 0;
  next_line_offset = 0;
  line_back_count = 0;
}

static void
update_display( void) {
  int i;
  char status[33];
  ClsTop();
  ClsBtm();

  /* print the line buffers */
  int num_to_print = num_active_lines - line_back_count;
  if ( num_to_print > 47) {
    num_to_print = 47;
  }

  for ( i = 0; i < num_to_print; i++) {
    int line_index = next_line_offset - 1 - i - line_back_count;

    if ( line_index < 0) {
      line_index += NUM_LINE_BUFFERS;
    }

    int y_value = 46 - i;

    if ( y_value < 24) {
      printtop_xy( 0, y_value, line_buffers[line_index]);
    }
    else {
      printbtm( 0, y_value - 24, line_buffers[line_index]);
    }
  }

  sprintf( status, "(line %d of %d)", line_back_count, num_active_lines);
  printbtm( 0, 23, status);
}

static void
print_string( const char *str) {
  int str_len = strlen( str);
  int num_lines = (str_len + MAX_LINE_LENGTH - 1) / MAX_LINE_LENGTH;
  int i;

  /* add the string to the line buffers */
  for ( i = 0; i < num_lines; i++) {
    int line_length = str_len - (i * MAX_LINE_LENGTH);
    if ( line_length > MAX_LINE_LENGTH) {
      line_length = MAX_LINE_LENGTH;
    }

    memcpy( line_buffers[next_line_offset], &str[i * MAX_LINE_LENGTH], line_length);
    line_buffers[next_line_offset][line_length] = '\0';

    next_line_offset += 1;
    if ( next_line_offset >= NUM_LINE_BUFFERS) {
      next_line_offset = 0;
    }
  }

  num_active_lines += num_lines;

  if ( num_active_lines > NUM_LINE_BUFFERS) {
    num_active_lines = NUM_LINE_BUFFERS;
  }

  if ( line_back_count != 0) {
    line_back_count += num_lines;

    if ( line_back_count >= num_active_lines - 47) {
      line_back_count = num_active_lines - 48;
    }
  }

  update_display();
}




/*
 * Keep everything inside the main while loop by sending events to it from
 * the interrupt handlers.
 */
uint32_t current_events;

#define TCP_TIMER_EVENT  (1 <<  0)
#define ARP_TIMER_EVENT  (1 <<  1)
#define WIFI_FRAME_EVENT (1 <<  2)

#define SEND_EVENT( new_event, events) REG_IME = IME_DISABLE; events |= (new_event); REG_IME = IME_ENABLE

/*
 * Get the stack pointer
 */
uint32_t getStackPointer( void) {
  uint32_t value;

  asm volatile( "mov %0, sp"
		: "=r" (value));

  return value;
}

/*
 * The lwip debug printer
 */
void lwip_debug_print( const char *fmt, ...) {
#if 0	
  va_list ap;

  va_start( ap, fmt);
  vsprintf( print_buffer, fmt, ap);
  va_end( ap);

  print_string( print_buffer);
#endif  
}

/*
 * Print the contents of a pbuf
 */
static void print_pbuf( struct pbuf *buffer) {
  int i;
  int offset;
  struct pbuf *cur_buf = buffer;

  while ( cur_buf != NULL) {
    offset = 0;
    for ( i = 0; i < cur_buf->len; i++) {
      offset += sprintf( &print_buffer[offset], "%02x", ((uint8_t *)cur_buf->payload)[i]);
    }
    print_string( print_buffer);

    cur_buf = cur_buf->next;
  }
}

#if 0
/*
 * The quickest timer event required by lwIP is 250ms hence this timer.
 *
 * TCP  250ms
 * ARP 5000ms
 */
static void regular_250ms( void) {
  static int count = 0;

  SEND_EVENT( TCP_TIMER_EVENT, current_events);

  if ( (count % (5000/250)) == 0) {
    SEND_EVENT( ARP_TIMER_EVENT, current_events);
  }
}


/*
 * The ARM7 has wifi data.
 */
static void data_ready( void) {
  SEND_EVENT( WIFI_FRAME_EVENT, current_events);
}
#endif

/*
 * Send packets into the lwIP stack.
 * This is similar to the dswifi code from WifiUpdate.
 */
static void lwip_packet_handler( int base, int frame_length) {

  /*  sprintf( print_buf, "Got a frame (base %d, length %d)", base, frame_length);
      print_string( print_buf);*/

  if ( !interface_active) {
    /* cannot do anything yet */
    print_string( "Not ready yet");
    return;
  }

  // if it is a non-null data packet coming from the AP (toDS==0) 
  if( ( Wifi_RxReadOffset( base, 0) & 0x01CF) == 0x0008) {
    /*  */
    u16 framehdr[12+2+4];
    int hdrlen_80211;
    int src_offset = 0;
    Wifi_RxRawReadPacket( base, 16*2, framehdr);

    if( ( framehdr[2] == ((u16 *)WifiData->MacAddr)[0] &&
	  framehdr[3] == ((u16 *)WifiData->MacAddr)[1] &&
	  framehdr[4] == ((u16 *)WifiData->MacAddr)[2]) ||
	( framehdr[2] == 0xFFFF && framehdr[3] == 0xFFFF && framehdr[4] == 0xFFFF)) {
      // destination matches our mac address, or the broadcast address.
      // base2 += [802 header hdrlen]+[slip hdr 8byte]
      if(framehdr[0]&0x4000) {
	// wep enabled
	/*base2 = base + 18;*/
	src_offset = 36;
	hdrlen_80211 = 28;
      }
      else {
	/*base2 = base + 16;*/
	src_offset = 32;
	hdrlen_80211 = 24;
      }
      // check for LLC/SLIP header...
      if( Wifi_RxReadOffset( base, (src_offset>>1) - 4) == 0xAAAA &&
	  Wifi_RxReadOffset( base, (src_offset>>1) - 3) == 0x0003 &&
	  Wifi_RxReadOffset( base, (src_offset>>1) - 2) == 0) {
	struct pbuf *new_frame;

	/* allocate the lwip pbuf and build an ethernet frame in it */
	new_frame = pbuf_alloc( PBUF_RAW, frame_length - 8 - hdrlen_80211 + 14, PBUF_POOL);

	if ( new_frame != NULL) {
	  struct pbuf *cur_pbuf;
	  int dest_offset;
	  struct eth_hdr *ethhdr = (struct eth_hdr *)new_frame->payload;

	  /* Create the Ethernet header in the pbuf (assumes pbuf is at least big enough for the
	   * header) */
	  Wifi_CopyMacAddr( ethhdr->dest.addr, framehdr + 2);
	  if(Wifi_RxReadOffset(base,6)&0x0200) {
	    // from DS set
	    Wifi_CopyMacAddr( ethhdr->src.addr, framehdr + 8); // copy src from adrs3
	  } else {
	    Wifi_CopyMacAddr( ethhdr->src.addr, framehdr + 5); // copy src from adrs2
	  }
	  ethhdr->type = framehdr[(hdrlen_80211 / 2) + 3]; // assume LLC exists and is 8 bytes.



	  /* skip over the newly created Ethernet header */
	  dest_offset = 14;

	  /* We iterate over the pbuf chain until we have read the entire
	   * packet into the pbuf. */
	  for( cur_pbuf = new_frame; cur_pbuf != NULL; cur_pbuf = cur_pbuf->next) {
	    /* Read enough bytes to fill this pbuf in the chain. The
	     * available data in the pbuf is given by the q->len
	     * variable. */
	    Wifi_RxRawReadPacket_offset( base, src_offset >> 1, cur_pbuf->len - dest_offset,
					 (u16 *)(cur_pbuf->payload + dest_offset));
	    /*read data into(q->payload, q->len);*/
	    src_offset += cur_pbuf->len;
	    dest_offset = 0;
	  }

	  /* see what to do with the frame */
	  switch (htons(ethhdr->type)) {
	    /* IP packet? */
	  case ETHTYPE_IP:
	    /* update ARP table */
	    /*print_string( "IP packet");*/
	    etharp_ip_input( &wifi_netif, new_frame);
	    /* skip Ethernet header */
	    pbuf_header( new_frame, -(int)sizeof(struct eth_hdr));
	    /* pass to network layer */
	    wifi_netif.input( new_frame, &wifi_netif);
	    break;
      
	  case ETHTYPE_ARP:
	    /*print_string( "ARP packet");*/
	    /* pass p to ARP module  */
	    /*print_pbuf( new_frame);*/
	    etharp_arp_input( &wifi_netif, (struct eth_addr *)wifi_netif.hwaddr, new_frame);
	    break;

	  default:
	    pbuf_free( new_frame);
	    break;
	  }
	}
      }
    }
  }
}

/*
 * The link output function
 *
 * A Copy of the similar DSWifi function
 */
static err_t wifi_link_output( struct netif *netif, struct pbuf *ether_frame) {
  /*print_string( "Wifi link output");*/

  /*int Wifi_TransmitFunction(sgIP_Hub_HWInterface * hw, sgIP_memblock * mb) {*/
  // convert ethernet frame into wireless frame and output.
  // ethernet header: 6byte dest, 6byte src, 2byte protocol_id
  // assumes individual pbuf len is >=14 bytes, it's pretty likely ;) - also hopes pbuf len is a multiple of 2 :|
  int base,framelen, hdrlen, writelen;
  int copytotal, copyexpect;
  u16 framehdr[6+12+2];
  /*sgIP_memblock * t;*/
  framelen = ether_frame->tot_len - 14 + 8;

  if( framelen + 40 > Wifi_TxBufferWordsAvailable()*2) { // error, can't send this much!
    return ERR_MEM;
  }
	
  framehdr[0]=0;
  framehdr[1]=0;
  framehdr[2]=0;
  framehdr[3]=0;
  framehdr[4]=0; // rate, will be filled in by the arm7.
  hdrlen=18;
  framehdr[7]=0;

  if( WifiData->curReqFlags & WFLAG_REQ_APADHOC) { // adhoc mode
    framehdr[6]=0x0008;
    Wifi_CopyMacAddr( framehdr + 14, WifiData->bssid7);
    Wifi_CopyMacAddr( framehdr + 11, WifiData->MacAddr);
    Wifi_CopyMacAddr( framehdr + 8, ((u8 *)ether_frame->payload));
  } else {
    framehdr[6]=0x0108;
    Wifi_CopyMacAddr( framehdr + 8, WifiData->bssid7);
    Wifi_CopyMacAddr( framehdr + 11, WifiData->MacAddr);
    Wifi_CopyMacAddr( framehdr + 14, ((u8 *)ether_frame->payload));
  }

  if ( WifiData->wepmode7) {
    framehdr[6] |=0x4000;
    hdrlen=20;
  }
  framehdr[17] = 0;
  framehdr[18] = 0; // wep IV, will be filled in if needed on the arm7 side.
  framehdr[19] = 0;

  framehdr[5] = framelen + hdrlen*2 - 12 + 4;
  copyexpect= ((framelen+hdrlen*2-12+4) +12 -4 +1)/2;
  copytotal=0;

  WifiData->stats[WSTAT_TXQUEUEDPACKETS]++;
  WifiData->stats[WSTAT_TXQUEUEDBYTES]+=framelen+hdrlen*2;

  base = WifiData->txbufOut;
  Wifi_TxBufferWrite( base, hdrlen, framehdr);
  base += hdrlen;
  copytotal+=hdrlen;
  if( base >= (WIFI_TXBUFFER_SIZE/2))
    base -= WIFI_TXBUFFER_SIZE/2;

  // add LLC header
  framehdr[0] = 0xAAAA;
  framehdr[1] = 0x0003;
  framehdr[2] = 0x0000;
  framehdr[3] = ((u16 *)ether_frame->payload)[6]; // frame type

  Wifi_TxBufferWrite( base, 4, framehdr);
  base += 4;
  copytotal+=4;
  if(base>=(WIFI_TXBUFFER_SIZE/2)) base -= WIFI_TXBUFFER_SIZE/2;

  writelen = (ether_frame->len - 14);
  if( writelen) {
    Wifi_TxBufferWrite( base, (writelen+1)/2, ((u16 *)ether_frame->payload)+7);
    base += (writelen + 1)/2;
    copytotal += (writelen + 1)/2;
    if( base >= (WIFI_TXBUFFER_SIZE/2))
      base -= WIFI_TXBUFFER_SIZE/2;
  }
  while( ether_frame->next != NULL) {
    ether_frame = ether_frame->next;
    writelen = ether_frame->len;
    Wifi_TxBufferWrite( base, (writelen+1)/2, ((u16 *)ether_frame->payload));
    base += (writelen + 1)/2;
    copytotal += (writelen + 1)/2;
    if( base >= (WIFI_TXBUFFER_SIZE/2))
      base -= WIFI_TXBUFFER_SIZE/2;
  }

  WifiData->txbufOut=base; // update fifo out pos, done sending packet.

  /* signal the ARM7 */
  IPC_SendSync( 0);

  return ERR_OK;
}

/*
 * The IP output function
 */
static err_t wifi_output(struct netif *netif, struct pbuf *p,
		  struct ip_addr *ipaddr) {
  /* add the ethernet header (getting the address from arp) and then send
   * the frame out using the link function */
  /*print_string( "Wifi ip output");*/
  return etharp_output(netif, ipaddr, p);
}

/*
 * The init function called from within lwIP.
 */
static err_t wifi_init(struct netif *netif) {
  return ERR_OK;
}

/*
 * Setup the netif structure for the wifi interface.
 */
static void setup_wifi_netif( struct netif *my_netif) {
  const u8 *mac_addr = Wifi_GetHwAddr();
  my_netif->name[0] = 'w';
  my_netif->name[1] = 'i';

  my_netif->output = wifi_output;
  my_netif->linkoutput = wifi_link_output;

  my_netif->hwaddr_len = 6;
  my_netif->hwaddr[0] = mac_addr[0];
  my_netif->hwaddr[1] = mac_addr[1];
  my_netif->hwaddr[2] = mac_addr[2];
  my_netif->hwaddr[3] = mac_addr[3];
  my_netif->hwaddr[4] = mac_addr[4];
  my_netif->hwaddr[5] = mac_addr[5];

  sprintf( print_buffer, "MAC address %02x:%02x:%02x:%02x:%02x:%02x",
	   my_netif->hwaddr[0], my_netif->hwaddr[1],
	   my_netif->hwaddr[2], my_netif->hwaddr[3],
	   my_netif->hwaddr[4], my_netif->hwaddr[5]);

  print_string( print_buffer);
}


/*
 *
 */
static void my_err_tcp(void *arg,
		       err_t err) {
  char what_error[30];
  struct web_connection *web = (struct web_connection *)arg;


  sprintf( what_error, "my tcp error %d", err);
  print_string( what_error);

  if ( web->current_connection != NULL) {
    web->current_connection = NULL;
  }
}

/*
 * The TCP polling
 */
static err_t
my_poll_tcp(void *arg, struct tcp_pcb *tpcb) {
  if ( tpcb != NULL) {
    struct web_connection *web = (struct web_connection *)arg;
    if ( tpcb == web->current_connection) {
      /* see if there has been any traffic */
      if ( tpcb->rcv_nxt == web->last_rx_seqno) {
	print_string( "Closing idle connection");
	/* close the connection */
	tcp_close( tpcb);
      }
      else {
	web->last_rx_seqno = tpcb->rcv_nxt;
      }
    }
    else {
      print_string( "Closing unserved connection");
      /* close the connection */
      tcp_close( tpcb);
    }
  }
  return ERR_OK;
}

/*
 * The TCP sent callback.
 * The stack has received acknowledgement for some data it has sent.
 */
static err_t
my_sent_tcp( void *arg, struct tcp_pcb *tpcb,
	      u16_t len) {
  print_string("my TCP sent");
  return ERR_OK;
}

/*
 * The TCP receive callback.
 * This is where data is read off on the TCP stream.
 */
static err_t
my_recv_tcp( void *arg, struct tcp_pcb *tpcb,
	     struct pbuf *p, err_t err) {
  struct web_connection *web = (struct web_connection *)arg;

  if ( p != NULL) {
    if ( tpcb == web->current_connection) {
      int copy_size = p->len;

      if ( copy_size >= 200)
	copy_size = 199;

      memcpy( web->rx_buffer, p->payload, copy_size);
      web->rx_buffer[copy_size] = '\0';
      print_string( web->rx_buffer);

      /* tell the stack we have received the data so it can advertise increase in the
       * receive window. */
      tcp_recved( tpcb, p->tot_len);

      /* process what we got */
      process_http( p, tpcb);
    }
    else {
      /* close the strange connection */
      tcp_close( tpcb);
    }
    pbuf_free( p);
  }
  else if ( tpcb == web->current_connection) {
    /* connection is closed */
    sprintf( print_buffer, "Connection from %ld.%ld.%ld.%ld is closed",
	     IP_ADDRESS_FORMAT( tpcb->remote_ip.addr));
    print_string( print_buffer);
    web->current_connection = NULL;
    web->last_rx_seqno = 0;
  }

  return ERR_OK;
}

/*
 * The callback called when a connection arrives for the web server.
 */
static err_t web_accept(void *arg, struct tcp_pcb *newpcb,
			err_t err) {
  struct web_connection *web = (struct web_connection *)arg;
  err_t accept_err = ERR_ABRT;

  print_string("SHIT\r\n");

  if ( err == ERR_OK) {
    sprintf( print_buffer, "New Connection from %ld.%ld.%ld.%ld",
	     IP_ADDRESS_FORMAT( newpcb->remote_ip.addr));
    print_string( print_buffer);
    if ( web->current_connection == NULL) {
      web->current_connection = newpcb;

      /* Setup the connection */
      tcp_arg( newpcb, web);
      tcp_poll( newpcb, my_poll_tcp, 10);
      tcp_recv( newpcb, my_recv_tcp);
      tcp_sent( newpcb, my_sent_tcp);
      tcp_err( newpcb, my_err_tcp);

      accept_err = ERR_OK;
    }
    else {
      print_string("One at a time please\r\n");
    }
  }

  return accept_err;
}


/*
 * Process key presses.
 * The X key is not processed here.
 */
static void
process_keys( uint32 keys_pressed) {
  if ( keys_pressed & KEY_A) {
    stats_display_pbuf( &lwip_stats.pbuf);
    stats_display_mem(&lwip_stats.mem, "HEAP");
  }
  if ( keys_pressed & KEY_B) {
    stats_display_proto(&lwip_stats.tcp, "TCP");
  }
  if ( keys_pressed & KEY_Y) {
    /* reset the display back to the first line */
    line_back_count = 0;
    update_display();
  }
  if ( keys_pressed & KEY_UP) {
    /* scroll back one line */
    line_back_count += 1;

    if ( line_back_count >= num_active_lines - 47) {
      line_back_count -= 1;
    }
    else {
      update_display();
    }
  }
  if ( keys_pressed & KEY_DOWN) {
    /* scroll forawrd one line */
    line_back_count -= 1;
    if ( line_back_count < 0) {
      line_back_count = 0;
    }
    else {
      update_display();
    }
  }
}


/*
 * All the lwip functions calls must be perform inside a single thread as they
 * are not protected as the stack is compiled with NO_SYS defined as 1.
 */
void test_lwip( uint32_t ipaddr, uint32_t netmask, uint32_t gateway) {
  struct tcp_pcb *web_pcb;
  struct ip_addr my_ipaddr = {
    .addr = ipaddr
  };
  struct ip_addr my_netmask = {
    .addr = netmask
  };
  struct ip_addr my_gateway = {
    .addr = gateway
  };

  init_print_sys();

  lwip_debug_print("Test debug printing %lx", getStackPointer());
  LWIP_PLATFORM_ASSERT("Test Assert");

  sprintf( print_buffer, "IP %d.%d.%d.%d", (ipaddr >> 0) & 0xff,
	   (ipaddr >> 8) & 0xff,
	   (ipaddr >> 16) & 0xff,
	   (ipaddr >> 24) & 0xff);
  print_string( print_buffer);

  /* the interface is not active yet */
  interface_active = 0;

#if 0  
  /* start a timer so we a call the arp and tcp timer functions at regular intervals */
  TIMER_CR(3) = 0;
  irqSet(IRQ_TIMER3, regular_250ms);
  irqEnable(IRQ_TIMER3);
  TIMER_DATA(3) = TIMER_FREQ_1024( 4);
  TIMER_CR(3) = TIMER_ENABLE | TIMER_IRQ_REQ | TIMER_DIV_1024;

  /* See when the arm7 wifi code has seen a frame */
  irqSet( IRQ_IPC_SYNC, data_ready);
  irqEnable( IRQ_IPC_SYNC);
  REG_IPC_SYNC = IPC_SYNC_IRQ_ENABLE;
#endif

  /* initialise the lwIP things - the order is important */
  stats_init();
  sys_init();
  mem_init();
  memp_init();
  pbuf_init();
  etharp_init();
  ip_init();
  udp_init();
  tcp_init();

  /* add the network interface */
  setup_wifi_netif( &wifi_netif);
  if ( netif_add( &wifi_netif, &my_ipaddr, &my_netmask, &my_gateway,
		  NULL, wifi_init, ip_input) == NULL) {
    print_string( "Failed to init wifi interface");
    return;
  }

  /* the wifi interface is the default */
  netif_set_default( &wifi_netif);

  /* bring the interface up */
  netif_set_up( &wifi_netif);
  interface_active = 1;

  /* add my packet handle to dswifi library */
  Wifi_RawSetPacketHandler( lwip_packet_handler);

#ifdef DO_WEBSERVER
  the_web_connection.current_connection = NULL;
  the_web_connection.last_rx_seqno = 0;

  /* create the web server tcp port (port 80) */
  web_pcb = tcp_new();

  if ( web_pcb == NULL) {
    print_string("Failed to create Web server PCB");
    return;
  }

  web_pcb = tcp_listen( web_pcb);

  if ( web_pcb == NULL) {
    print_string( "Failed to listen on web pcb");
    return;
  }

  /* setup some callbacks */
  /*tcp_poll( web_pcb, my_poll_tcp, 10);
  tcp_recv( web_pcb, my_recv_tcp);
  tcp_sent( web_pcb, my_sent_tcp);
  tcp_err( web_pcb, my_err_tcp);*/

  /* Pass the web connection data around the callbacks */
  tcp_arg( web_pcb, (void *)&the_web_connection);

  /* wait for connections */
  tcp_accept( web_pcb, web_accept);

  if ( tcp_bind( web_pcb, IP_ADDR_ANY, 80) != ERR_OK) {
    print_string("Failed to bind to HTTP port (80)");
    return;
  }
#else
  /* lets see if we can connect to a web server */
#endif

  while(1) {
    uint32_t my_events;

    /* read and clear the current events */
    REG_IME = IME_DISABLE;
    my_events = current_events;
    current_events = 0;
    REG_IME = IME_ENABLE;

    if ( my_events & WIFI_FRAME_EVENT) {
      /* let the wifi code update causing the any frame to appears on to the
       * packet handler */
      Wifi_Update();
    }

    if ( my_events & TCP_TIMER_EVENT) {
      /* call the lwIP tcp timer function */
      tcp_tmr();
    }

    if ( my_events & ARP_TIMER_EVENT) {
      /* call the lwIP arp timer function */
      etharp_tmr();
    }

    scanKeys();

    uint32 keys_pressed = keysDown();

    process_keys( keys_pressed);

    /* pause execution (could leave things in a funny state) */
    if ( keys_pressed & KEY_X) {
      int i;
      for(i=0;i<8;i++) {
	BG_PALETTE[i+1]=RGB15( 31, 0, 0);
	BG_PALETTE_SUB[i+1]=RGB15( 31, 0, 0);
      }

      do {
	scanKeys();
	keys_pressed = keysDown();

	process_keys( keys_pressed);
      } while ( !(keys_pressed & KEY_X));

      for(i=0;i<8;i++) {
	BG_PALETTE[i+1]=RGB15( 31, 31, 31);
	BG_PALETTE_SUB[i+1]=RGB15( 31, 31, 31);
      }
    }
  }
}

