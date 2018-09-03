; Auto-generated. Do not edit!


(cl:in-package ssddetect-msg)


;//! \htmlinclude BoundingBoxes.msg.html

(cl:defclass <BoundingBoxes> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (boundingBoxes
    :reader boundingBoxes
    :initarg :boundingBoxes
    :type (cl:vector ssddetect-msg:BoundingBox)
   :initform (cl:make-array 0 :element-type 'ssddetect-msg:BoundingBox :initial-element (cl:make-instance 'ssddetect-msg:BoundingBox))))
)

(cl:defclass BoundingBoxes (<BoundingBoxes>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <BoundingBoxes>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'BoundingBoxes)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name ssddetect-msg:<BoundingBoxes> is deprecated: use ssddetect-msg:BoundingBoxes instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <BoundingBoxes>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader ssddetect-msg:header-val is deprecated.  Use ssddetect-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'boundingBoxes-val :lambda-list '(m))
(cl:defmethod boundingBoxes-val ((m <BoundingBoxes>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader ssddetect-msg:boundingBoxes-val is deprecated.  Use ssddetect-msg:boundingBoxes instead.")
  (boundingBoxes m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <BoundingBoxes>) ostream)
  "Serializes a message object of type '<BoundingBoxes>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'boundingBoxes))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'boundingBoxes))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <BoundingBoxes>) istream)
  "Deserializes a message object of type '<BoundingBoxes>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'boundingBoxes) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'boundingBoxes)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'ssddetect-msg:BoundingBox))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<BoundingBoxes>)))
  "Returns string type for a message object of type '<BoundingBoxes>"
  "ssddetect/BoundingBoxes")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'BoundingBoxes)))
  "Returns string type for a message object of type 'BoundingBoxes"
  "ssddetect/BoundingBoxes")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<BoundingBoxes>)))
  "Returns md5sum for a message object of type '<BoundingBoxes>"
  "468565c3a90acd3f0e4aa67421b39e1f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'BoundingBoxes)))
  "Returns md5sum for a message object of type 'BoundingBoxes"
  "468565c3a90acd3f0e4aa67421b39e1f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<BoundingBoxes>)))
  "Returns full string definition for message of type '<BoundingBoxes>"
  (cl:format cl:nil "Header header~%BoundingBox[] boundingBoxes~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: ssddetect/BoundingBox~%string Class~%float64 probability~%int64 xmin~%int64 ymin~%int64 xmax~%int64 ymax~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'BoundingBoxes)))
  "Returns full string definition for message of type 'BoundingBoxes"
  (cl:format cl:nil "Header header~%BoundingBox[] boundingBoxes~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: ssddetect/BoundingBox~%string Class~%float64 probability~%int64 xmin~%int64 ymin~%int64 xmax~%int64 ymax~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <BoundingBoxes>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'boundingBoxes) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <BoundingBoxes>))
  "Converts a ROS message object to a list"
  (cl:list 'BoundingBoxes
    (cl:cons ':header (header msg))
    (cl:cons ':boundingBoxes (boundingBoxes msg))
))
