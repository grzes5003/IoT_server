use std::net::{SocketAddr, SocketAddrV6, IpAddr, Ipv6Addr};
use cty;
use cty::c_char;


#[repr(C)]
pub struct message_t {
    pub in_msg_t: cty::int8_t,
    pub token_t: cty::int64_t,
    pub payload: [c_char; 8]
}

#[no_mangle]
pub struct sensor_t {
    _addr: SocketAddr,
    _msg_arr: Vec<message_t>,
    _next: Option<Box<sensor_t>>
}

impl sensor_t {
    fn new() -> sensor_t {
        sensor_t {
            _addr: SocketAddr::new(IpAddr::from([192,168,0,1]), 0),
            _msg_arr: vec![],
            _next: None
        }
    }
}

#[no_mangle]
pub extern "C" fn sens_find() -> Box<sensor_t> {
    Box::new(sensor_t::new())
}

#[no_mangle]
pub extern "C" fn print_stuff() {
    println!("stuff hello");
}