..  _data-model:

================================================
Data Model
================================================

As a very typical kind of applications we can apply our pipelines of complex transforms in either computational
advertising scenario or recommendation system scenario.

In these domain applications the data is composed of a user
with his/her real time context followed by a list of candidate advertises. Usually the number of candidate advertises
varies from hundreds to thousands depending on system's computational capability and ability of upper recall systems.


.. image:: _static/auc-type.png

Due to the frequency usages in above mentioned application scenarios and the consideration of implementation
efficiency, we define our data model in this way.

In our designed system the data has 3 statuses: attribute data, feature data and encoded data. Here is the definition
of attribute data with some commonly used types (e.g., categorical, set categorical, real valued and time).

.. code-block:: Java

    message AUCImprAttrs {
        repeated float labels = 1;
        AttributeBunch user_and_ctx = 4;
        repeated AttributeBunch ads = 5;
    }

    message AttributeBunch {
        map<string, AttributeData> attrs = 1;
    }

    message AttributeData {

    enum AttributeType {
        UNKNOWN = 0;
        CATEGORICAL = 1; // string, int_val
        SET_CATEGORICAL = 2; // SetCateValue --> [<string, float>]
        REAL_VALUED = 3; // float
        UNIX_TIMESTAMP = 4; // int_val (1546235518)
    }

    AttributeType attr_type = 1;

    oneof oneof_attr_val {
        string str_val = 2;
        int32 int_val = 3;
        int64 long_val = 4;
        float float_val = 5;
        SetCateValue scate_val = 6;
    }

    message SetCateValue {
        map<string, float> vals = 1;
    }

And the following is the definition of feature data with discrete type and continuous type.

.. code-block:: Java

    message AUCImprFeats {
        repeated float labels = 1;
        FeatureBunch user_and_ctx = 4;
        repeated FeatureBunch ads = 5;
    }

    message FeatureBunch {
        map<string, DiscFeatGrp> disc_feats = 1;
        map<string, ContFeatGrp> cont_feats = 2;
    }

    message DiscFeatGrp {
        map<string, DiscFeatData> feats = 1;
    }

    message ContFeatGrp {
        map<string, ContFeatData> feats = 1;
    }

    message DiscFeatData {
        string gname = 1;
        string fname = 2;
    }

    message ContFeatData {
        string gname = 1;
        string fname = 2;
        float fval = 3;
    }

The definitions are implemented with Protobuf format which is very suitable for data passed across different kinds of
systems with different implemented containers and programming languages. The users can check the data-model-protobuf
library for real definitions.

