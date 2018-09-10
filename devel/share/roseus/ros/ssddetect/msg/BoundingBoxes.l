;; Auto-generated. Do not edit!


(when (boundp 'ssddetect::BoundingBoxes)
  (if (not (find-package "SSDDETECT"))
    (make-package "SSDDETECT"))
  (shadow 'BoundingBoxes (find-package "SSDDETECT")))
(unless (find-package "SSDDETECT::BOUNDINGBOXES")
  (make-package "SSDDETECT::BOUNDINGBOXES"))

(in-package "ROS")
;;//! \htmlinclude BoundingBoxes.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass ssddetect::BoundingBoxes
  :super ros::object
  :slots (_header _boundingBoxes ))

(defmethod ssddetect::BoundingBoxes
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:boundingBoxes __boundingBoxes) (let (r) (dotimes (i 0) (push (instance ssddetect::BoundingBox :init) r)) r))
    )
   (send-super :init)
   (setq _header __header)
   (setq _boundingBoxes __boundingBoxes)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:boundingBoxes
   (&rest __boundingBoxes)
   (if (keywordp (car __boundingBoxes))
       (send* _boundingBoxes __boundingBoxes)
     (progn
       (if __boundingBoxes (setq _boundingBoxes (car __boundingBoxes)))
       _boundingBoxes)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; ssddetect/BoundingBox[] _boundingBoxes
    (apply #'+ (send-all _boundingBoxes :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; ssddetect/BoundingBox[] _boundingBoxes
     (write-long (length _boundingBoxes) s)
     (dolist (elem _boundingBoxes)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; ssddetect/BoundingBox[] _boundingBoxes
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _boundingBoxes (let (r) (dotimes (i n) (push (instance ssddetect::BoundingBox :init) r)) r))
     (dolist (elem- _boundingBoxes)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get ssddetect::BoundingBoxes :md5sum-) "7b3add6f6b751ce1e4ba418832812a17")
(setf (get ssddetect::BoundingBoxes :datatype-) "ssddetect/BoundingBoxes")
(setf (get ssddetect::BoundingBoxes :definition-)
      "Header header
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


")



(provide :ssddetect/BoundingBoxes "7b3add6f6b751ce1e4ba418832812a17")


