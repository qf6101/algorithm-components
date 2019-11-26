..  _config:

================================================
Configurations
================================================

This section describes the configurations of attribute transform and feature transform.

------------------------------------------------
Attribute Transform
------------------------------------------------

The method of attribute transform supports the four types of attribute data as follows:

(1) Categorical Attribute.

(2) Realvalued Attribute.

(3) Time Attribute.

(4) Set-Categorical Attribute.

The next and also important thing is how to define attribute transform above four types of attribute data.

.. code-block:: xml

    <attribute in_aname="input_attribute_name"
               out_gname="output_group_name"
               out_fname="output_feature_name"
               procs="func1;func2"
               params="func1_param1,func1_param2,...;func2_param1,func2_param2,..."/>

As shown above example of xml format, we need to configure five elements that are "**in_aname**", "**out_gname**",
"**out_fname**", "**procs**" and "**params**" regardless of categorical attributes, realvalued attribute, time attributes
or set-categorical attribute.

There are some brief explanations about above five elements as follows:

**in_aname**:

 Attribute name of input data.

**out_gname**:

 Group name of output which after attribute data transferring.

**out_fname**:

 Feature name of output which after attribute data transferring. It just need to configure symbol "%" for
 categorical attribute, set-categorical attribute and time attribute and symbol "=" for realvalued attribute since the
 program can generate the feature name after attribute data transferring by parsing corresponding symbol automatically.

**procs**:

 The functions of process attribute data which are different for processing different types attribute data.
 Them are separated with symbol ";" like "proc_func1;proc_func2" if there are multiple different functions.

**params**:

 Parameters corresponding to multiple different functions which also are separated with symbol ";"
 like "*func1_params*;func2_params*". Parameters of a function are separated with symbol ","
 like "*func1_param1,func1_param2,...*". If a function has no parameters, it just need to configure symbol "-".

(1) Categorical Attribute
------------------------------------------------
The following is an example of xml format which is used for configuring categorical attribute.

.. code-block:: xml

    <categorical-attrs>
        <attribute in_aname="userSex"
                   out_gname="user_sex"
                   out_fname="%out_gname/user_sex%male:1.0"
                   procs="prune_max_len"
                   params="skip,10"/>
    </categorical-attrs>

**out_fname**:

 Just configure symbol "%" or string "%your_comments" since the program just parse fisrt char of the
 string and the left chars of the string are ignored.

**procs**:

Only one function:

 **prune_max_len**: Pruning maximum length.

**params**

The parameters of the function **prune_max_len**:

 (1) "**skip, max_len**" such as "skip,10".
 The **max_len** is maximum length of attribute value.
 The **skip** represents that the program will skip the attribute
 if the length of attribute value exceed **max_len**.

 (2) "**cut_head, max_len**" such as "cut_head,10".
 The **cut_head** represents that the program will cut the attribute value from head to **max_len**.

 (3) "**cut_tail, max_len**" such as "cut_tail,10".
 The **cut_tail** represents that the program will cut the attribute value to tail with **max_len**.

There is another one example of xml format about **params** as follow:

.. code-block:: xml

    <categorical-attrs>
        <attribute in_aname="userSex"
                   out_gname="user_sex"
                   out_fname="%out_gname/user_sex%male:1.0"
                   procs="prune_max_len"
                   params="cut_head,10"/>
    </categorical-attrs>

(2) Realvalued Attribute
------------------------------------------------
The following is an example of xml format which is used for configuring realvalued attribute.

.. code-block:: xml

    <realvalued-attrs>
        <attribute in_aname="userAge"
                   out_gname="user_age"
                   out_fname="=out_gname/user_age:26"
                   procs="prune_range;scale_min_max"
                   params="skip,5,70;5,70"/>
    </realvalued-attrs>

**out_fname**:

 Just configure symbol "=" or string "=your_comments" since the program just parse fisrt char of the
 string and the left chars of the string are ignored.

**procs**:

The functions of process realvalued attribute value as follows:

 (1) **prune_range**: Pruning over range.

 (2) **log**: Log computation.

 (3) **scale_min_max**: Min-max scale.

 (4) **scale_z_score**: Standard scale.

**params**:

The parameters of the function **prune_range**:

 (1) "**skip, min_value, max_value**" such as "skip,0,1e6".

 (2) "**replace, min_value, max_value**" such as "replace,0,1e6".

The parameters of the function **log**:

 (1) "**log10, min_neg_val**" such as "log10,-10".

 (2) "**log, min_neg_val**" such as "log,-5".

 (3) "**log2, min_neg_val**" such as "log2,-1".

The parameters of the function **scale_min_max**:

  "**min_value, max_value**" such as "0,200".

The parameters of the function **scale_z_score**:

  "**mean_value, std_value**" such as "0,1".

(3) Time Attribute
------------------------------------------------
The following is an example of xml format which is used for configuring time attribute.

.. code-block:: xml

    <time-attrs>
        <attribute in_aname="happen_time"
                   out_gname="=in_aname/happen_time"
                   out_fname="%out_gname/happend_time%weekend:1.0,happend_time%raw:1534204864,..."
                   procs="all"
                   params=""/>
    </time-attrs>

**out_gname**:

 Just configure symbol "=" or string "=your_comments" since the program just parse fisrt char of the
 string and the left chars of the string are ignored.

**out_fname**:

 Just configure symbol "%" or string "%your_comments" since the program just parse fisrt char of the
 string and the left chars of the string are ignored.

**procs**:

The functions of process time attribute value as follows:

 (1) **all**: Transform with all time level.

 (2) **partial**: Transform with partial time level (e.g., without hour level).

 (3) **time_str**: Time string format if input data type is string.

**params**:

The parameters of the function **all**:

 using symbol "-" since it is parameterless.

The parameters of the function **partial**:

 a combination of one or more of "**hour, day, week, month**" such as "week", "hour,week", "hour,day,week" and
 so on.

The parameters of the function **time_str**:

 time and date formatting symbols such as "%Y-%m-%d %H:%M:%S", "%Y-%m-%d" and so on.

(4) Set-Categorical Attribute.
------------------------------------------------
The following is an example of xml format which is used for configuring set-categorical attribute.

.. code-block:: xml

    <setcate-attrs>
        <attribute in_aname="poi_cpt"
                   out_gname="=in_aname/poi_cpt"
                   out_fname="%out_gname/poi_cpt%ent:0.3,poi_cpt%sport:0.7"
                   procs="prune_range;norm_sum"
                   params="skip,0,10;-"/>
    </setcate-attrs>

**out_gname**:

 Just configure symbol "=" or string "=your_comments" since the program just parse fisrt char of the
 string and the left chars of the string are ignored.

**out_fname**:

 Just configure symbol "%" or string "%your_comments" since the program just parse fisrt char of the
 string and the left chars of the string are ignored.

**procs**:

The functions of process realvalued attribute value as follows:

 (1) **prune_max_len**: Pruning maximum length.

 (2) **prune_range**: Pruning over range.

 (3) **norm_sum**: Norm sum scale.

**params**:

The parameters of the function **prune_max_len**:

 (1) "**skip, max_len**" such as "skip,100".
 The **max_len** is maximum length of attribute value.
 The **skip** represents that the program will skip the attribute if the length of attribute value exceed max_len.

 (2) "**cut_head, max_len**" such as "cut_head,100".
 The **cut_head** represents that the program will cut the attribute value from head to **max_len**.

 (3) "**cut_tail, max_len**" such as "cut_tail,100".
 The **cut_tail** represents that the program will cut the attribute value to tail with **max_len**.

The parameters of the function **prune_range**:

 (1) "**skip, min_value, max_value**" such as "skip,0,1e6".

 (2) "**replace, min_value, max_value**" such as "replace,0,1e6".

The parameters of the function **norm_sum**:

 using symbol "-" since it is parameterless.


Finally, there is a complete attribute transform example of xml format as follow:

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>

    <aucimpr-attributes>

        <user-and-ctx>

            <categorical-attrs>
                <attribute in_aname="userSex" out_gname="user_sex" out_fname="%"
                           procs="prune_max_len" params="skip,10"/>
            </categorical-attrs>

            <realvalued-attrs>
                <attribute in_aname="userAge" out_gname="user_age" out_fname="=out_gname/user_age:26"
                           procs="prune_range;scale_min_max" params="skip,5,70;5,70"/>

                <attribute in_aname="userCost" out_gname="user_cost" out_fname="=out_gname/user_cost:100"
                           procs="prune_range;scale_z_score" params="replace,0,1e6;150,50"/>

                <attribute in_aname="userCost2" out_gname="user_cost2" out_fname="=out_gname/user_cost2:100"
                           procs="prune_range;scale_z_score;log" params="replace,-2000,1e6;1.5,1;log10,-500"/>
            </realvalued-attrs>

            <setcate-attrs>
                <attribute in_aname="poi_cpt" out_gname="=in_aname/poi_cpt"
                           out_fname="%out_gname/poi_cpt%ent:0.3,poi_cpt%sport:0.7" procs="prune_range;norm_sum"
                           params="skip,0,10;-"/>
            </setcate-attrs>

            <time-attrs>
                <attribute in_aname="happen_time" out_gname="=in_aname/happen_time"
                           out_fname="%out_gname/happend_time%weekend:1.0,happend_time%raw:1534204864,..." procs="all"
                           params=""/>
            </time-attrs>

        </user-and-ctx>

        <ads>
            <categorical-attrs>
                <attribute in_aname="productId" out_gname="product_id" out_fname="%out_gname/product_id%abc:1.0"
                           procs="prune_max_len" params="cut_tail,150"/>
            </categorical-attrs>

            <time-attrs>
                <attribute in_aname="happen_time2" out_gname="=in_aname/happen_time2"
                           out_fname="%out_gname/happend_time%weekend:1.0,happend_time%raw:1534204864,..."
                           procs="partial;time_str"
                           params="week,month,day;%Y-%m-%d"/>
                <attribute in_aname="happen_time3" out_gname="=in_aname/happen_time3"
                           out_fname="%out_gname/happend_time%weekend:1.0,happend_time%raw:1534204864,..."
                           procs="all;time_str"
                           params="-;%Y-%m-%d %H:%M:%S"/>
            </time-attrs>
        </ads>

    </aucimpr-attributes>



------------------------------------------------
Feature Transform
------------------------------------------------

The method of feature transform supports the two types of feature transform as follows:

(1) Unary Feature.

 (A) cont2cont

 (B) cont2disc

(2) Binary Feature.

 disc2disc

The next and also important thing is how to define above two types of feature transform.

.. code-block:: xml

    <op-ctx in_gname="user_cost"
            in_fname="=in_gname/user_cost"
            out_gname="#in_fname/user_cost#log"
            out_fname="=out_gname/user_cost#log"
            op="log"
            scope="fname"
            procs="log10"
            params="-"/>

As shown above example of xml format, we need to configure eight elements which are
"in_gname", "in_fname", "out_gname", "out_fname", "op", "scope", "procs" and "params".

There are some brief explanations about above eight elements as follows:

**in_gname**:

 Group name of input corresponding to "out_gname" in Attribute Transform.

**in_fname**:

 Feature name of input corresponding to "out_fname" in Attribute Transform.

**out_gname**:

 Group name of output which after feature transferring.

**out_fname**:

 Feature name of output which after feature transferring.

**op**:

 Feature transform operation.

**scope**:

 Scope of feature transform operation.

**procs**:

 The functions of process attribute data which are different for processing different types attribute data.
 Them are separated with symbol “;” like “proc_func1;proc_func2” if there are multiple different functions.

**params**:

 Parameters corresponding to multiple different functions which also are separated with symbol “;”
 like “func1_params;func2_params*”. Parameters of a function are separated with symbol “,”
 like “func1_param1,func1_param2,…”. If a function has no parameters, it just need to configure symbol “-“.

(1) Unary Feature
------------------------------------------------

A. cont2cont
+++++++++++++++++++++++++++++++++++++++++++++++++

The following is an example of xml format which is used for configuring cont2cont feature transform.

.. code-block:: xml

    <cont2cont>
        <op-ctx in_gname="ad_cost"
                in_fname="=in_gname/ad_cost"
                out_gname="#in_fname/ad_cost#log"
                out_fname="=out_gname/ad_cost#log"
                op="log" scope="fname"
                procs="log" params="-"/>
    </cont2cont>

**in_fname**:

 Just configure symbol “=” or string “=your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**out_gname**:

 Just configure symbol “#” or string “#your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**out_fname**:

 Just configure symbol “=” or string “=your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**scope**:

(1) **gname**

(2) **fname**

**op**:

 **log**

**procs**:

(1) **log10**
(2) **log**
(3) **log2**

**params**:

 using symbol “-” since it is parameterless

B. cont2disc
+++++++++++++++++++++++++++++++++++++++++++++++++

The following is an example of xml format which is used for configuring cont2disc feature transform.

.. code-block:: xml

    <cont2disc>
        <op-ctx in_gname="ad_revenue"
                in_fname="=in_gname/ad_revenue"
                out_gname="#in_fname/ad_revenue#bkt"
                out_fname="@out_gname/ad_revenue#bkt@1"
                op="bucketize" scope="fname"
                procs="predefined"
                params="ten-equal-width-zscore"/>
    </cont2disc>

**in_fname**:

 Just configure symbol “=” or string “=your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**out_gname**:

 Just configure symbol “#” or string “#your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**out_fname**:

 Just configure symbol “@” or string “@your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**scope**:

(1) **gname**

(2) **fname**

**op**:

 **bucketize**

**procs**:

 (1) **customized**

 (2) **predefined**

**params**:

 (1) **customized**

  **va1, val2, ..., valn** such as "1,2,3,4,5,6,7,8,9,10"

 (2) **predefined**

 (a) **ten-equal-width-zscore** such as "ten-equal-width-zscore"
 (b) **ten-equal-width** such as "ten-equal-width"

(2) Binary Feature
------------------------------------------------

A. disc2disc
+++++++++++++++++++++++++++++++++++++++++++++++++

The following is an example of xml format which is used for configuring disc2disc feature transform.

.. code-block:: xml

    <disc2disc>
        <op-ctx in_gname1="all" in_fname1="%in_game1"
                in_gname2="all" in_fname2="%in_game2"
                out_gname="!cross^in_gname1^in_gname2"
                out_fname="!cross^in_fname1^in_fname2"
                op="crossall" scope="gname" procs="ad-inner"/>
    </disc2disc>

**in_fname**:

 Just configure symbol “=” or string “=your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**out_gname**:

 Just configure symbol “!” or string “!your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**out_fname**:

 Just configure symbol “!” or string “!your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**scope**:

(1) **gname**

(2) **fname**

**op**:

(1) **cross**

(2) **crossall**

**procs**:

(1) The procs of op **crossall**:

    (1) **ad-inner**

    (2) **uc-inner**

    (3) **ad2uc**

    (4) **ignore_uc_gnames**

    (5) **ignore_ad_gnames**

    (6) **ignore_uc_fnames**

    (7) **ignore_ad_fnames**

    (8) **ignore_double_cross**

(2) The procs of **cross**:

    using symbol “-”

**params**:

The procs of op **crossall**:

(1) **ad-inner**

 using symbol “-”

(2) **uc-inner**

 using symbol “-”

(3) **ad2uc**

 using symbol “-”

(4) **ignore_uc_gnames**

 **name1, name2, ..., namen** such as "sex,age"

(5) **ignore_ad_gnames**

  **name1, name2, ..., namen** such as "sex,age"

(6) **ignore_uc_fnames**

  **name1, name2, ..., namen** such as "sex,age"

(7) **ignore_ad_fnames**

  **name1, name2, ..., namen** such as "sex,age"

(8) **ignore_double_cross**

  using symbol “-”

B. cont2cont
+++++++++++++++++++++++++++++++++++++++++++++++++

The following is an example of xml format which is used for configuring cont2cont feature transform.

.. code-block:: xml

    <cont2cont>
        <op-ctx in_gname1="user_cost#log" in_fname1="=in_game1/user_cost#log"
                in_gname2="ad_cost#log" in_fname2="=in_game2/ad_cost#log"
                out_gname="!cross^in_gname1^in_gname2/!comp^user_cost#log^ad_cost#log"
                out_fname="!cross^out_gname/!comp$avg_dot^user_cost#log^ad_cost#log"
                op="comp" scope="fname"
                procs="partial" params="avg_min, avg_max, avg_dot"/>
    </cont2cont>

**in_fname**:

 Just configure symbol “=” or string “=your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**out_gname**:

 Just configure symbol “!” or string “!your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**out_fname**:

 Just configure symbol “!” or string “!your_comments” since the program just parse fisrt char of the string and
 the left chars of the string are ignored.

**scope**:

(1) **gname**

(2) **fname**

**op**:

 **comp**

**procs**:

(1) **partial**

(2) **all**

**params**:

(1) **partial**

 a combination of one or more of (**avg_min, avg_max, avg_dot**) such as
 "avg_min,avg_max", "avg_min, avg_max, avg_dot" and so on.

(2) **all**

 no parameter

Finally, there is a complete feature transform example of xml format as follow:

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>

    <aucimpr-feat-trans>

        <user-and-ctx>
            <unary-trans>
                <cont2cont>
                    <op-ctx in_gname="user_cost"
                            in_fname="=in_gname/user_cost"
                            out_gname="#in_fname/user_cost#log"
                            out_fname="=out_gname/user_cost#log"
                            op="log" scope="fname" procs="log10"/>
                </cont2cont>

                <cont2disc>
                    <op-ctx in_gname="poi_cpt"
                            in_fname="%in_gname/poi_cpt%ent"
                            out_gname="#in_fname/poi_cpt%ent#bkt"
                            out_fname="@out_gname/poi_cpt%ent#bkt@1"
                            op="bucketize" scope="gname" procs="predefined"
                            params="ten-equal-width"/>
                    <op-ctx in_gname="user_age"
                            in_fname="=in_gname/user_age"
                            out_gname="#in_fname/user_age#bkt"
                            out_fname="@out_gname/user_age#bkt@1"
                            op="bucketize" scope="fname" procs="customized"
                            params="0,18,25,30,40,45,50"/>
                </cont2disc>
            </unary-trans>

            <binary-trans>
                <disc2disc>
                    <op-ctx in_gname1="user_sex"
                            in_fname1="%in_game1/user_sex%male"
                            in_gname2="user_age#bkt"
                            in_fname2="%in_game2/user_age#bkt@1"
                            out_gname="!cross^in_gname1^in_gname2/!cross^user_sex^user_age#bkt"
                            out_fname="!cross^in_fname1^in_fname2/!cross^user_sex%male^user_age#bkt@1"
                            op="cross"vscope="gname"/>

                    <op-ctx in_gname1="all"
                            in_fname1="%in_game1"
                            in_gname2="all"
                            in_fname2="%in_game2"
                            out_gname="!cross^in_gname1^in_gname2"
                            out_fname="!cross^in_fname1^in_fname2"
                            op="crossall" scope="gname" procs="uc-inner"/>
                </disc2disc>
            </binary-trans>
        </user-and-ctx>

        <ad>
            <unary-trans>
                <cont2cont>
                    <op-ctx in_gname="ad_cost"
                            in_fname="=in_gname/ad_cost"
                            out_gname="#in_fname/ad_cost#log"
                            out_fname="=out_gname/ad_cost#log"
                            op="log" scope="fname" procs="log"/>
                </cont2cont>

                <cont2disc>
                    <op-ctx in_gname="ad_revenue"
                            in_fname="=in_gname/ad_revenue"
                            out_gname="#in_fname/ad_revenue#bkt"
                            out_fname="@out_gname/ad_revenue#bkt@1"
                            op="bucketize" scope="fname" procs="predefined"
                            params="ten-equal-width-zscore"/>
                </cont2disc>

            </unary-trans>

            <binary-trans>

                <cont2cont>
                    <op-ctx in_gname1="user_cost#log"
                            in_fname1="=in_game1/user_cost#log"
                            in_gname2="ad_cost#log"
                            in_fname2="=in_game2/ad_cost#log"
                            out_gname="!cross^in_gname1^in_gname2/!comp^user_cost#log^ad_cost#log"
                            out_fname="!cross^out_gname/!comp$avg_dot^user_cost#log^ad_cost#log" op="comp"
                            scope="fname"
                            procs="partial" params="avg_min, avg_max, avg_dot"/>
                </cont2cont>

                <disc2disc>
                    <op-ctx in_gname1="all" in_fname1="%in_game1" in_gname2="all"
                            in_fname2="%in_game2"
                            out_gname="!cross^in_gname1^in_gname2"
                            out_fname="!cross^in_fname1^in_fname2"
                            op="crossall" scope="gname" procs="ad-inner"/>

                    <op-ctx in_gname1="all" in_fname1="%in_game1" in_gname2="all"
                            in_fname2="%in_game2"
                            out_gname="!cross^in_gname1^in_gname2"
                            out_fname="!cross^in_fname1^in_fname2"
                            op="crossall" scope="gname"
                            procs="ad2uc;ignore_uc_gnames;ignore_double_cross"
                            params="-;feat1_grp;true"/>
                </disc2disc>

            </binary-trans>

        </ad>

    </aucimpr-feat-trans>