// Auto-generated. Do not edit!

// (in-package ssddetect.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let BoundingBox = require('./BoundingBox.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class BoundingBoxes {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.boundingBoxes = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('boundingBoxes')) {
        this.boundingBoxes = initObj.boundingBoxes
      }
      else {
        this.boundingBoxes = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type BoundingBoxes
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [boundingBoxes]
    // Serialize the length for message field [boundingBoxes]
    bufferOffset = _serializer.uint32(obj.boundingBoxes.length, buffer, bufferOffset);
    obj.boundingBoxes.forEach((val) => {
      bufferOffset = BoundingBox.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type BoundingBoxes
    let len;
    let data = new BoundingBoxes(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [boundingBoxes]
    // Deserialize array length for message field [boundingBoxes]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.boundingBoxes = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.boundingBoxes[i] = BoundingBox.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    object.boundingBoxes.forEach((val) => {
      length += BoundingBox.getMessageSize(val);
    });
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'ssddetect/BoundingBoxes';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '7b3add6f6b751ce1e4ba418832812a17';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
    BoundingBox[] boundingBoxes
    
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    # 0: no frame
    # 1: global frame
    string frame_id
    
    ================================================================================
    MSG: ssddetect/BoundingBox
    string Class
    float64 probability
    int64 xmin
    int64 ymin
    int64 width
    int64 height
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new BoundingBoxes(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.boundingBoxes !== undefined) {
      resolved.boundingBoxes = new Array(msg.boundingBoxes.length);
      for (let i = 0; i < resolved.boundingBoxes.length; ++i) {
        resolved.boundingBoxes[i] = BoundingBox.Resolve(msg.boundingBoxes[i]);
      }
    }
    else {
      resolved.boundingBoxes = []
    }

    return resolved;
    }
};

module.exports = BoundingBoxes;
