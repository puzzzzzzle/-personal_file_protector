//
// Created by tao on 19-3-25.
//

#include "get_usr_passwd.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include "log_init.h"
#include "AES_func_adaptor.h"
#include "get_usr_passwd.h"
#include "file_key_controller.h"
#include "../crypto_adaptor/AES_func_adaptor.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <hash_adaptor.h>
#include "main_passwd.h"
#include "common_includes.h"


#define MAM_LINE_LEN 500
#define MAX_ROW 30
static const char randCharTab[MAX_ROW][MAM_LINE_LEN + 1] = {
        "NscpnsFp2KVfWNq0y54NCczqLg6YEqU0dRKe1zBD9zVnQSAMb79o2tYTTrNKdpr4AwNdTwEjEOSuTDgBdNgkPbnx5u9nZWckeLJ9V1DYKY7Gp1ElT6UsKgwUrYMuXMpJTQ9c2GjSfligQckAwevCXcozgp6mPqhcReRG9RsSYKJjnYSfmNorKfKkRtSCXunSnOGK4o5wnMrMlrWcyRRWiTTh56z6sQouzCPrOB7nqVWgNpZ2fFvbHYnURuudqETqnnbqyLP4ldwr60Fnb8Y7nvfrq9iXb4gM8HQphBzQzGxcd9IHzDqtcLWsRV14AWQ7Q4noJR0PMksZJYH32GRWRigJbgKBETIuhz0jkrojNOBDptDnnzSwedVdUUIiHMTzXmeTLZRI8P2LaItrVkA4diH8BlsOwlyZXYP1FEJn5oZlI5J4RmSV1Ha0GeqiBJEtNlfkqJhPqEVh1Mh7r5wzqC6YMP8QdyHMYWtsrrcVe6ALCzzcrfU3",
        "FjhNqyqZfKgyemf1Hh74wIAp6y22cXUpaQJcKd9jED98yqPQBwCjLddX54JExnfcu19hFtnqqDuMcESEApxwugryjr2sXyRvCew9D291d7Hya1P5NIH6ETftV0Zr4OR6aZLoUCCjMHnKaZx85XgHV5TaTnsHcw1UIyYOpu91gKmOlaIsEWY3YCA4BcYcBdnlfr2u39S80PsQEmvz29BreiXMuZLcrSK3EDtudpDPmTJBiabDSS029K1zkPe20KO1qGKBt74WKVQK63JNqnozpTt5UkNkuBoZhnEFUMeomWVihCZzpt58OpRoQkOXMuZlbjZ905nobmB0TFQJe1uVufUjcZmD6vIH1DiqdjAqV80vRcETORkD1icHSfBQk3mB59uYuyDS9qdxralV00pgd8J7vLuYGfsz3dI73tA75X7Dih7h4BOsxggn81OQrRQUXnho00ivpCA9iCjPTf8hRSqtzOZWBq8YDrFuhg0wuQvFWW8Rl3sp",
        "pAToiM0WkbAPjPMVxdZ04SROHbk0b7zzh9WHTmxzqOauR6ZPukWuDi33yuAOvStCXcdJUR4v9co247SJqpCpHY7WLseVsRa7rSIlkQImStOnXw0FwASe2NDR6LiY1LWx7udj6LWu46vxe3fZgPCKy7UG4Dz8gdl4WSjEOExrOkRqH8QPd61lt4yryTuo9rLPkFfWJ8lcZ3zPodsq8KOjBQEZes0kg8V3RskxfhVqKYZalLPhJ8PT4Rht2osY1ki8cF0sv1WzprtcHhJph3oKpD7EblrbUC7UXk9LcS5mLoIOnsIL5kYIIIVnusD3B3ZeD6HChmJgJ6NzoavDspfNGJyGVfDTJ3YAeZLESEUsRJtM5WolWXVQIapFwmGTIeg6MzjShqWYYDLj7M1GiQfCb3kO0V5wSwzacEl5GfhXvxM1osBbZQyHcysYQuLUCMckXRtV6OSpvejurbEL7y5GGM2IP8RnmUxu9Xa4E8L63uS6IYlAYsnM",
        "qZ0pnnzljfInw45RM2lC2w99OV1NTGBeZXhR9wBvBcmvfT2cR5q4cfnILHxaItorVUIDPZmYtXvmBYoNpekMIUmirOd4I3J7Jc1oqxLxEkIxJUNbWlJ8lNgoE5k964eLe6qhWIghR1aU6j45zJIci3CyArQT5AKaUuuD0cLEBFZn1UM61ed5bSkg6C3OLBMHZsSoDhEcQXe6Tu2QkiuRWdmTTzFWB84Vtvdo204ND8a9oQh92buHXPoATae60THXD08HIWvvLpe37G2OISuaBvg1QtVjGxhwvu05P7qooTBX9hidYpnHfiiCk0VjvTbcrO5iJzo8UeLnetz4LrDEAa2uRzPSegp427IS6C6vU1f4ka2UzPrXqwQdiqDNhifNbz37wmkcWuocWLeDYNaqb7YXatptwxJf01H8ZuEUGJwBm3rEJCx0xofNqn1KBxg2GAQYWh6sigrogfV5U2Rp5kt5DynVUkaPohi9hwRRuHXnUBZQfgn6",
        "5NsQnZJ9eZigJUOrj1jLtZZsOvsIjizAszPs6l2yvnsxjj2GDeE0j8U6tc8f0S8CdUtnT9FzsyIfPcH0EU8STh19Tg5AaOD0grncjZIXQYH9JQGHkpi0T6HbDdTDDiF5HA3cahw7VNzcXRhxjj424Vlhs5JOXTkRLnyKyD1Abxz3IPKQkSStovRYXGa20mQGsOB1Wq1lnj58S2gIlQeWPPlpO9dT3rEuPqJl3ny1YZYB826WdJiAzLGU8ZyYIRdibKFqnpm4D4AVsBkP7iSBmYl7nFiBXU4J64uCeFPQDdLv0NCRpaOpqVavgxa6rLylehiIV4Haaf076upgctC6LVb1pKK2hrYDpqi0ZhmlyOk2Aq3NbhsxSLASsBe018DexW7JUo8s4pYMWcurBdECWJEWWdEkWUPFodXcas4dp5VbgpDPC3656GXVDI5CrizzdYwgSIUd9eXQiT4RSUdbjbRwMIzERvYdng0qkJ59fKyeLfkOXGO1",
        "XcV1UJ7f3yzWe5eb6HaZUzCq2N3Joi0ox70aYkh26Znyr5A2ECAbkTWKXyDtdNViOFuAGhGA5kD0daa9VZdLRm4jgQGzk4v8GHIowSwMbALJICBLuZ9BdokwDGnjjpYjMPKkBZLtEVHP5tOXTVzIgad2mgf0ClY8jcAPyibIprHjkAI8HMunGLRJhaTaWfj3ZodFrbrXtmIJCPA1kCCZK2qUOcSHd4iibqp86mU0R4nCUdBVb8pl5rofsXlF5AQ4uMi06qfWjaAvAm4UnDr3C87CW0y2uO8KVB43z9hpLGWBu85HIFZnUgERYsHUGG4120OttCizO5gjiVYlEzfjh8yQBylIwJDcL1NBCcr0icCVyXjhxOBP1lXG5Itff822uOlF5M8VYIjTye77Awf1clT4uyXr5hig8Y4GiOlchGyGRD9pYMVs6JZlaUACJQbZgYKWRNqsAaAx6ndTlBorcEDJPY5LMs8ckgdFA6QTnSdqMWmneSq0",
        "Azc4WV5BQgiM69h8ZxuUVD0Bn4tk6iEdwvQUS587kjN1OpT9eslXgvAvEPjWDAtXzmXKWmHWZ1te5XdZZm5HdNmOWx9QLjmeS28EtdK9NZHmYZRpsbrGRocHpFU8XNJWlkjH5lIiAwClRzbfWUHL5kc9zs3TGFBoEKQDO7pobwRSYraN2otXmPgq7T0mzKYkNHpeNHr2zV8IUWhwp50ebirGhUKHjUgRqWfyxPSzbYtGyESl4tSGna7yuBVw3MAy6GjGidpkz0qmvojb4JPeQoI2JiPXXfQf96o1KyKdkUdGwx68LhFKJ085rguqJ8sgASCEUsSuA0RNe51abyBjvaxMvZ0Ae72k1pBhHWfclVA4p2aKXJ9K2uVBuAudBI6DFeWdP0nnqlSwf4KdLrzzvhZUp4fIO82oqYEjtoMV977ayCwYtuF6SuGPLfY70CUOAYj3ZBKeYrCWn96RQv6YQxQUjzzv9yZnZlV7rca3szqMVbvh1Dx6",
        "Je6v20MiGFxvwCDYBGLAWx8avvdkMqQ9USkho02F9VrD7a79HueHq7oKhIKDsWsVatv9h4rUldCmgOi3FlVp6kpt1nSynaU8mtMxwZse5g4ZccX4FOY7FUyJ8Ywl4Of9KOcAHnAnEsPAcIbKzIEqC3ZILmQTO0YTko9XRjdUXZkModP6uoaDrc3lrRC8ypTF6y19Gu78scPKFOP3raMZuT05tiCv01jCdA4Nm3TC4v7cSXY8OtiXBfuxgf17LGjrsyRV16VaAuRWNogg26iDtaPC3qUBmqFL4rshaXRwkt5uSYMPwrRnTitb2ZbBCACqdplqe9BeyeJ4poww7SGkmSvNozGmJte3WyLcwGTZdfs6q8gUMwXysoswVd9dXxZCXcR7bcR7y2omdSwcOwKOE8WEPHubwS0YkaewqHG29jt8LZ1LrWeFHT2zqhV8wxhC7TkuR24l78O5vLdNbII7xjcW0j7NjxIaLG3wmuoxKhHMBssEANHd",
        "exKtMRM3dcyibJj6gJH1QgtJyqsEx47oi84iHQzEJ01FQY6TExWzmfe8mm0OFKdZuQs3B51EC3dbnenc2hd6aQwCtjHR6M56zFBpSD0MePFKROhRvGGD4qFWM0okNfHjSFtKhe7lVelSwg1jxvS0DjcE1jtdtFk5OygXz5pTSQHveOC3nSCWeBux8eLiQtjKY6XMJRKjrYVLrSr5rA5A8CR7vykaZ7BoR9um0yJ4GWdfIOryHwSVcf23YJoqMtas4XumFdPhn6RIqQN72qcg2A1QzlPo9v6XykdMIJ6gEZTI6xpIO86mJgHnLfK1XIx0K281rasLBZHUNtMhTdxPyhcPbNFMUGyQs9CjLu3m6AAyKPfrQlWkH2geXWy8vHZTql4m5j70KO5tPohcF3um7w5Zb0BCyzGqptqFPIob7EkeYch7i9e3F4ZL3Jo82rU5MbInxhajIZyKlveXyoYTaFTBbWE9VpWduAORDx6se4eYUHtrUOCn",
        "vN3zngMaBbfaRWalTYXbovNSoaeDWJGXv4NlJvUl1VUEt0IARwQZNc7LfjSp8oTIc6NPvLqeIDbmbOHvbV9hI5n7TWzxNKl7YVcEPulJhqmfzvnPEQCLawTXYDSB6dpJRlETSsLreA6bOwqHbessUpDANg5LXcVDWnIWPXrw8sDdbllrI7uzohrCi7nba0CqsrWgjrDORKlwCRV59BAiw2mSk2chPsKBwr6Ga0qGNev2jaatAqbVl9tm2uooMuxsAjMK0dvb84hecHHWjcNHU29dqOYOfoTBHl91L4c8KEhyX0BVGdWvTZN4qAMqhTm9sMQZceU6L9KfAOiVCx5A9CaUebgPFcQTt4bibT4R4qYqEcLRItGolOg9yQbPZKVqTiqYw33iC0ePKD1L0wnlXdP2DtOXCdfZa0EP2xNVqMrpwUj6pU2BsYzu6Mze9CvPiEEcul4HBwXanxhBVni2uVlZGTwXi5mMQONconbu9KoznkFVWp9U",
        "Gav4IbgTe3or4njK31Pgz8mYqPUZ7fxcv82aUHDV5ZQjkFT0fbiAClYpGOkEsVtpg8D6Tr7gf1JK8cPFAuxKxPfsIKfTgc70hdCLgd9vx21OlGAzVDVCyl8jaWUPupb6hg1WenXZqakhfTYxIBAHrkvVX8WGeTYT3rz06oTZId6AEEwMntpLWkmceDiHkiQFHM3BazkKO6BUo5hfJjqSPWwvGEOhENA0z0xfqH9ZPC0cv7okmSIMCzG8qxBzZAfwmIcVNKZlrgt3yp5GffYeaLcEIdMAsVB1oTVSYYMrLJWydGimCjGRdwLafkY4BnHi1Q00rLBhs7wpwjvXXzKIt7DpFTlsAMFC2EDxDjGc5I97sMi5EALF83zpSD1OiIdB6wZRlZ5rDHcHCkso5yD4wYbniGOcQsfGxDbWpffVWMzcRLBrmGhcAUdaMny71rxASbw7JR78euW14JzLaqKUAC68Ne3NpnkbSXi0DrHKLlvGpn0RnuqX",
        "ha42EefNiFj45WMwEZLz5uCsllrCarE0CNBo7lwU9UmeEDybweivfxJaortMbZAHHKPG6fNzVUWCH7QbPFjIYu6xH0SBfkIpkY4iyRN7OvUWkiX3Mldb0IS7KzNBEe7qYB9auJ5wb3yf5Kh49o3GwEHVoNXR9EedGQ4qXq70SMmKB4LdItnaLkUuNsU1iKGWk3LWw3z1lntqsZfdCc4Klb1tMP0q4bnpqB9H1WTOqhvkqicyS6bs6OmZAd93ibJrfZlDcnzlCmb2CHdDK7Rxy5GK5yEH1q0TcieH2Bpddr58umDfboLPO3h7ciS4DAiGMyxjWj1q7p8GS5DGnTusPiotzPZUmCF3yBY7sw70m80zsdIVFKlrCTiTBiA1YoZRJJ2zpACfy1pgxkCDYjT8Rk0b4aestzZD0Ax2KrF4FKBW1FbS85sVliXfFNjKW88d9cMgRRtjNNYUxlsuBL736AQjFsfbwdSLSr1tIVns3GuDsARPD9wx",
        "o1J7jCNvHidd3DEUc0F36uaAbYXK2YGBl90RD7dNcETJg4yke0DA05yQrJfjykCdwmMuGDfM3oRkYWIGYfkChOQoLPROPuRcrZ3Zdd83QkhdYqOJ19U0K867MXshwAdEzSTSKO9Ez4z7sdCoDgVUr0PAPVO6C4y0KzDV74CPI8sLLeRpjAxXIPohYwayuEYzUIASmJj0pWp20zqFGuu6HcSl8elyYMhIbXAhWPZ5u7wPssNp5dt6DxuVjvG3PL5GicliHr51P2DvzhrfvVa7sYNCQnJwQbk7DhC2O7pT5JEVDWnGsSLIRLNJVIpkgb7s6tP5H93ttS9eYRPWqa4iBrpf4aCtWWUP0l1BQI8T9WGoh54JrTcFclmuhgwsDQr7yU8kPUOemwwbJJk0EtyP5WZYuKZMJVrHgOOZi7p8bqrPY3z3QeweekUg8p1PBWD9P3PG830P2T1o7YCmTpa0KojRDIZgcppLmtqQBXXaWp2tskozdcjy",
        "OaccUsjsGzPruzTX4jTz66Xkp7ORO0xwrbbcfhhaiGiSDNAMGiqvZxPEnDxDUaiFm77czmeJpxhIIilpBY5vscZ84a7z0gyCUUlXVZRDt48JiEo7BClcw9xHTczEHm7mQnnQwfLztKj7bKfTlbzauCrkkOhjDCiJtMmFeSxoteuWOkjOOprS5e9dCKBvrLqvbfDUBVn3UuEAebTYhriFCnQ8VheWRRynt43d7Y5enOhy9n3VXuFY0USa8Qk67nITCA6RP0Z48ou7tpRQRK2X8AWbevYPG5SlYqyolvBQplKzx1O7bLy2V5SWcz8NNlTMd0QOZJT2N2oiMENdysncuW55pkqDwuTeKVttMvHH6BYa57tPLxMwyJBoFviBsp1csX9XgsqdzMinQJt3pTBpXlWQP3HymgcvgyAIn548U7qX9CBhS9br9RikbvucRGeQQWIf51C1hzndew5m41O1nCp7BimahBW4RuVbX8ybibZJFCUSG7Zy",
        "h5mAFJbCYggIWwXeRTT04ypfDiELVC1PshLvbI6kNLbi3H8J7RzXM5TW3OPuQlVgRPrCxaMo4ixaG2S1iWLpL7P1gKRfoGGnP7Sk27YzAD6S5CWqvLjKDFvl8loZrGziazClX3ctogsRQB4YGAG5OkUqVhnz5iKGIuCsuumhAEs5kjui2y6iX9lsX7l9XJWL6PTIbL0efhlViqna7X8lNa9NMzx0rOyglBajhRXCF2OSZLS4kq6g0vEmixy7cwxQ8VY0B7faiClHIjuyt8VgYNAtjBMRSsz9JNocwrOn2SGQWHnZttQETFQvjoF6ynwmPeJs5YCxgRpwISUfXPY8iX51twY61B5Jjc4s8y1dsBC1oP29zyT0VGsQ2h736GRAn9EHr1SvxNfQKoxCe1iruYSaMt012YJdvETPKkJVmrUkPEC8G4Hl4zXWQ28NuvJGwcSXQF66Vyh5ZnNVidIgoxO4XyMIoODXwruDLhTquBqf7Nb33Ghk",
        "3TyLHHv6gd7MjFXTPBJsqJUYGbAysbTk2d00Vj19wsXbIdY561pmduNsg0zzfFVTPpNpRh9AeJCy1tEVbwgMhFLCzW7JKk7m7FlWuD4DD82d3udf903GVLLhIMV8img4qejs4ckQzl29uSn4tb1nyPhYtiG4UMKGTUN41NNVjsjRsuN4IBPPtSKvd1IJzqQjg3GtfvYoyzcJrJGOSedisevyuTdQZDpWet5O5c57CIJCtCB1NIeLQoey99ZRjTR9jKxsP5pYrcuto9hhm9TNM3kkHr5rxD1qqDjrrG9CcgC0r4IpdB9wb2f3uqJblK6S0qFfCqpla0k0qixkjRputrHdZ8HkxlFuiPfFkdEIaJH4EVQIoQqcfPuRXSWRdHv4eAZMl0qYE2daYXsjZaDOn9ZuW7KaoFtPqp1DSclt54h5DWkbyAnPSUDmhJcFvHG6TEwYDKlNyTZlryKVnmZ7rRWAg5uNw4s4unM6eSQCbAdgK7ie2W2I",
        "KPfQ2miSB6BRiJS43zviKSPaqU9lsL4lJ3KVIEbddVJciN0ZrHL2ImWjF01drumnDBx00H2hN9X8QJNJ8PT8HSLlQomKNSDnCmpk8grhA0jC9O1ApdcFINOD4GSVdF9Xnu8a2QnfAmmZRQR4YAXTdG9hYjiE8moxOgSQwkpVtDP2qFOzzr3rw39Md50n9Cd0lF9vaHhRmkJu0202FLzw0wwFoSxPxqbjSzvqs61TTOZkCX3sBXxKpXzUzMtTCrky2J5Wr8hGHpfTi7j7cloKts2wM3stFGtMSfEahV3ND9ObemOrObKIxkuP5lfKnO3gHyXsUkcBcW6NJXfuCVFxzRrDQvYHDavIezKSWRWdqy0460795MZ2iOuXTdmZ0r55YNU7nS9kIlrPsTSkcCm97KpahM3bkzrS6pWpfDQSolZZxWZeLoHORJY0cLRtW3aidbyk4X3FcRJwnGzWh5xGJd0uL2KP2v7yhwen6cWRBByIyIX60iMT",
        "DU4Tq6Bf4tsCNE8breQgMj3jNnw17OkHOiOrVL630s1H6m0kHu4gArX4DqsjgAMgeCAiYkgNWy4ExhwnsCFtPdmPaPjNuJ5700UK1hNb8VaUB2dmdjEzkd2JYwSX3U5Ul5TDxDzKlqz4NgxEehqSUugOH7otWu7rxC2u1uic0BuAQfxjP4JBwYEiFzUIkWDPocA7oMFltIZj7aTvbDljNfqwOBpwhyJLxOW8Cy7PSBhbYZvRTlCEjJz5f4GMwW1tlGRE3W32nuVpu4dzcbGsw7mWOSXARAz0anMjoxXo9Jte9YdEIE0FymJbh1HwSAtrGlhABMV09m9Hv9YNISQnm983pltZyAx5kdWaXT8r39buoAh8aW3BAGLpSSRBac2yEiPvEanKbrgXYF9QwYXRTQhlhH5f1m4PWVsxRzzu01dnM8H98V5QKfuSrHH9QE5nMNfeJ2P0D07JVkAAr9sEVfkkx1mcLJIGIIMuWMj6MoiDspQnDswI",
        "LjBrQlPde9fS41sP5E3eSUotAmrkUmt0zOSqvU56bKvvLs8LLk1h0EmaK3dvSEXuSgxUN3RI4hAGzrwA28xhD6IiMU6Ij8CdgFlddLDuGL8qZcYvVcnHq0MKV8TZGBjWSaokI4iof6pH4XQ5yFaUlmmDyySw7EkPSKFl3NdkSBgLKzH3QVLCnl2PP9KLluSWlCxhwMPOy6Z0bkxonb07hQi7Ir6DOyLErW3bHFqGw9KrSx2xZqq5oHv01M0neEEHIPjcte0Q1yroJviQlsOXrF1CTnzNIfHWDR1lKTGLefwt7ZELSV9b06FEwWdIl6bnWxnOnbDR4fO56JMvNsRNBfWIyQymnACPs8rczaD2pN6ismJdJXW2SzbGK8vA6n2gWjmieE1aL3aaE7MxDHRpOPiTZsfmZO8IjzI5zdqfXeb9hjYKScje2hoD4W3Hzfx6MGv66yaoa64lHWATZhR9GqPdP4cKoY9glTN6SxCUT1WSENIqIqm6",
        "OItAddeelWTveSzAKi02xzQ9O9fbAN81RFqraMrnLbwMaoMhSKUH9FSp1rMsVMGpMvbb8r6CyiUjIhBcNi7nheJKrQ4xRHfvBIXF4KS3qQiX2q50uQguseUjwhuPSh71yoOtokuJHiJQZVP3JnF3W5Q7z9o09GmbsczIVXxiM4JM2jznmazgIL2lMgH8Dhl7LfVB32w9cgpDwXM0x2fYQpkYQKPi8DgxpncK2inxzF6A4OnyMVOKj4IzKhc5mZm5XW7dDL15wMaZubZVwWhX70cXM9yLWheOQECAXfj4qBcBQDKJrDlkK3jgUNq8ULWnGFjorfeg74wxXTpaPJrMVAdxMv81mmh9ADEF1tYmnIN2fKLG5kvcIJsEjHrNunWoNSkuUyzSvJdnEfItrJfcKtTqthzgY3ySlD2jFgSFOUayeOtuUekpelG4FZHYeDyBY4CRPr3bSAF83Mjj6bTbfgHF4gJWZ5SRPWHGRGQDpL8KZ8fHUAr0",
        "fwxuP9f5V0NhjZfNymsP2bv0pMTLF7nBSUI8mX578q8MQ6slg5IioAKifrFg5HwcA6w5jpPXZnFLJOSTWrfpGqmwXhWRLhoCqi9GNV9bYcLwRREfAmPSOYTXbEcrrc4H0Y8tMNyFtnAKbhauTW0p5uDgGRTQaP5t3Oe8myNfc19lQqBkhHhognDIKFGbYBG68OKwmkX4o2WzS2QDzYEJK2HJ7enXn0hePDvymglKEfX8gPONRyr6cHn6Qusez38Mav1KGBx7curQsF1zZd5DfROTJksadXjA0BQpoqcYL2XjiBCCpWsz7o0Bv9fiFfUtEowsiPSZVY5GbVuSWdLA63yN0RMQ3JPfu4wlWzCcH92yu1D74fGe6EmF0XGt4zAY7jmVPtGQ3ELclrFRbgPWAwTmMJX6YuY2T1lxuSWBgezg4N8CuGig20fRjTJzDz6qCaca03vDpAqzFZHALyrSOsG5ABeMJLc6UlxR6MQzWgyLIyI1dfCW",
        "MAHT8imnzqUurqs6kns0hmax7mlJyWi3wNIELR7W0MWkS1aC9CjYIof7m2sF5KrgetW9ixh6gX4AE4NJ54OFIjYINlo1j7NdophZjeF0CuYEqII4hRNWmDvpcoeigds3E0M6VaaWKrB5PcxqaW4DMR7Z895z5qFyBC2iJr5S8FR6GOHKcmuXORoN9lGIuIVXhjRrXse8ZZOpBg3Kz4OSeMo1zE7kV3NjvuwI1ZvEBBO4aselQBpa8p9J7P930WKpkS6x7P0HAVEpxJwMqei0EjwaHH9jqFWDWA3dAEQE6fArG3KvlVJpY4LWvBGVsX4W2xwBEacMgscM1rjBOUfUnux4yoe3QY2B4jvxAzHZannWCuqkKAfOORKH8xWnccuNEurvt9aS6pxn67aDxM8UfR62OxpgndA8pzo9tAoHNOQiPnkATSODSJRbVriqjQ0hp3Eks4iqmvEdOIHYtrjxJSUanpTR4GM6OpiWQRrBa7CoX4uRbeJl",
        "2fpChqJofoUhYhla9kE5UjVrfXx2usHF67nkAb96IxdzoCcAS4ULORj5AzwtEwOX0yA7sj7A9ZSj5KAA0OXVOYYx48rm0EhDvIcA0oyzZwdPsIXE8MD1vG6nXdwZdXXslABfXCAsA7Rw67TrJDLVYBm4myuECCHlz90mpI9mrvGv61TFbAF4gOiC3YY5qMnS4S9ed4UZR2taM1QT1wu61d7Ea0mvY8Cn6CIdJTkYABTSlrC0ZfaT0rTp52BDTbFODTIEpRr3b82mIWaRXoYuXO4rSFUyBSwjYYdIToYdz3XxdQvaIbXNahjhxTH741rcvtB1XkUuIQGw2srNqd4D0u19BiAbVNwytyXLkRyrL2KF4WoPLbP8QFhImTOL0iCIhQcmAkYnPQBNTED5Rw2kXNdtJlu3HGtH1fOMKHM1Jfyw3mCUInjxNHCy9jrwXVzgP1DJTHG3oZND3LJkMDJCOpI3ZGZD8rGtcQx9HlXEWFCiHV40hOt6",
        "E9kgaaJXWpcrpojF9IwqtA3D7yC4dF7NoTRbO2VfF05yb6eQYPOXrdjjPobHfYyoZEDaSsCbnd3mwP9yTgYGTUGQfQsDCLjbBBfdYSHu5dPFZv2HP04Lkc5A0zk3YrjeAHV6Dbqz9Jj9s9AGLLeCMOPmcFc1qMFxSHVpA6SUyaZbMyzkrnd50Yyt43ZlMHAK2fbMBppyFQu1LIv4nAJuE3L6K05HF3R3VJQLG3vbecJK4HBUjwWlFnmAu4814iNaG290vvmaJEKUjqD7cUXZZOZDDbLNMsGAIJczRhXznnDVBxFinWUAMjKipLgAHgIcM3ubYJMQPjAMYhxIeyP8X3AaSUpFrWyUccXMRpP9zmSgAnX449VWjaNyP6104hg1Fqlu4HjUZkwQ5AmcRNZmJyH0p7ww89kbLK3g9U4Fq0HHL1byUS02Qu8S5BZvynDq6UDEqulpGaFsj71GIY8bJtAW8F5ghNyUH8N45Tg73y0jzNxekxAc",
        "kaVJZz8Z99Mnqes5VUHxvL4U9fpVNkMdI0KwoOZ6OXpNzjrrVA66KZegrhfisc3JgojfwSCGpqkXWjHma7AxMYavY9VRgoVav0bmGEwTbmYMjU47OPaG5SG0qnH5EGLLuVLYTwOit8X4BjqmrFodG3J2nzb3Ahervy47UNXqmDMtjClCRgMm2zASzOk9o8qYgKzRFGMZhP7j8SwA7ifT8gj30ANflRi8eKqTKFeoHl9DBRetii9ifInN8GFw8baCBIcSrTaHxvMgcm4ECLE5ui7PRMjxkxh4ZFQovrd0TvQAwuRLA3TT2iZaKAg3woMnDvpPkBbKOUkuD6knHESxx53MQfPaTE5kKGgLhzD54ExkH1ONUCipjZT5P6CnC7bveOYhveUD8cJEXbQ4TQDd3uOOyl2AXTzoxhbzef33ZnQl4oi6tbCgKg4FzZYbtugmnO70Ip22NU2hzzmqpjWue4VUp6ymoHzXkfbAzwTVMJwzgt3Gm9G7",
        "180zsxwMkchbtJbAOfwxcXhA7pwGaSVbMbyPVUNz0O8PLBTupgT5rRFPvtun02z1K3ExTe09FqrchHC5erI0LbIxtU8QOUtii7JAhQL9XSiBC6TC5ebmXSc4Cmt5uX0I0UWcoPfNduUCwicZm6kQ0vEMAa8y0v0698nwM9f2kUwacwacSRmniRzJbEnPvInYQhe2OT8xbRXZ5qordPYxgNZWTZ0YDHKlghfLJVkAAJz1g4JLDu9XWXDuFstGY9ydIDoet41o4aq8ACobbdoAle0jH3ttIQUHrSusXbHTPndAJt0b68Yd6ah00btgFvCMwqxNhzcKa1ck6Dk1duZxSICKtMXkEyl4RqBYGd2GF6dfhaOzZ5F6PPJmlNYhdq7aOwZzAXQfGvT6rVHf0TsbvsqZjNmIYQ3PpHXEgdDrvopgcQctQ7VJTZzjJfdmmfteLJitwbHawIHF3g8R6LqqbhDP6If5LMqhgmhmJJH4HLhLq6gTByO4",
        "KBS9dJwRHSO2s6PCjCr3N2TfzOEQWgRfpo3mIYFw3m8jHnsBfnsbcWn9Ne39H3x97EywojyXsQLgqPFt949cpHNKxY9YNiZYKwOnPAAlMpUwc6TIChuxj2sZ93IFfBpAM8JNwys3ZEZsl4WZdTYy6IdVusAFtAybnftkmt4TRrJonH20nagvQjyiOqN5u3HEISRgBsD4nvMFH6KNdEjoQyfcyOppjHyGdcR3DLShFtrJ0rdvzLLwR9YN49p5i86cXFr0kpORKiUo9YacgAroJycmrV5W7UtfhKCt3MQ85LWvcXYgyCDZebXfD5GxZhtkeT9yr7WQyB1u86sfnR3Usa7VcxgaqsPyvlvnciXDetPU5GMpe5RByeFG1g84fwSSFCg52ZbjUg9GczMD0UHbkZArdpVQLjZSb4r3hDE8SUFTW9UxVG4bHgI4CIGyc3WXgp7F0nr6MyCJLots2FCcNy5Vxk0HKYyOa0Je1X9GwhCIJF3f15cJ",
        "WcbmEK7XyzjI9BnIu6RBfXo3cxP6iWzRTat8keC7VvQazIP8KFl6Y5R3kvhbXFHbzMnq2veP35P9368vTXHKOQzI3JFzrKITiJJfn8w2L1HStTAYtFJY9GSEOeg2Rd22CXHCXyo1Zo0nh8njwrUVA6r8pHy8ugQV98aLHE7jNkFGvAoqo1MRR7zLpU8DBomuz2W921MHDLZoLjZ3FAZmSSDv7hroywPqBmExO1lEldjxgIp947pKc4aCts2PooIC3ak6KyAl7Y2d1vCH6UtkjlZcJGpqQU6kGWGqZwLzdOMt53jd0IJdOSGTRGoDuaaVTq6hYtr27p7LXJKu4lhQFgmT2HCEs3yWwTbxXWoHeknXNd9vwOaf60HEbKOcCLgBKzmXCzokGmeEiiyvfuGvnDfwrLllzLVGiPJVj7DO27HvvXBD0bDZK29SMrEyiHgiYRMTYnE6ueyty7b1vR1Iv6g9eJua1aVWg69OcssmAZukaXOIy77P",
        "Sv4d2pl1VRRxkrdAprFZspNAdMECPdBhyiG0rElfz7gSVWM7F0DR5tKWhazLr6g36IHmfNBaypn7HhbdU0du47jx2dTRM3pjPTFyF3SSkf2AFw1jQ0ScjQ0R5tkfWCsiMOfGiDVulPcGlZ91xYYoyRvOnCNiXaNGFunOPZKjD9W8Tz8qz2PaKOCtexyja20mikiB9FnZJfSRv7RNTEfEQde9sYozOkFVfnd0RnUPa7uW6OyeXbHYxtU2ua6qIdp1FmsolZTNY1fesQqZtCAkVaEJQFmOz7HOlJHY5h7JCLAm4uslQuAjSLDCQNdJfN4jwzj5tfm3FdRehfLC1KRZYN1tZ2tJQDvhrMtmX9eSS6exbC7sb59Wzx9EqYuX2w3bAu4EJoyDnhwCQtIPvzuCC9XxzOSb1G69on6bvTGSx7bgvf8hY6EEFRI7GmzoS06THZL07r8n3fht0jDAlT8Xb2eEhL4GgBZz1qL1YQNV8GSN8Gbx4von",
        "wOwn5jeh7KOQjg6VIMMHuhVhEeAD1Z68H9JwmfmQNC02jD7rwdEeeXU8617fjMLVyAYejL7FM69ytx3uDZfcLW6UBadEY6utrN3HfSEfoVafN6EZYHYWjM9A125bYl0PV9n1kYQJPnXwC8z1CKkRsDQAYnr3X0VgCnH6wm64f5QUC7y1E8AQvbH6Zezm199FoTY1ZlhG2VA6o0YAgwg4BjkeU9NziLLDYZQayk1EqsF6AEYtJhgn33RuJ1guVEb9p1pxwuZjdWdG7toloBQwxanZtpiuRHP7nXwvtzoO3zIJ0fAZgvhhgxyyTMei0dzh7IaYW5kkG2BpaPW41pDpvTLsocx9Xc79fga54cPcFWi7Mk5sImyNBsUMBMkVBTjJV2RfFmmv2PFPjw6MXBD2OZ6YRApAGdaT8l9wTfazpqC31RuD2aWiyaBhzHXxhEz7rO9SL72y4CK06AcBVASYHhHhsPAq5srPOKLo8oZxG8iy9nrzplDo"
};


UserKeyTable *UserKeyTable::Instance = new UserKeyTable();

std::mutex UserKeyTable::lock{};

#define USER_KEY_PREFIX "./sysdata"


std::string get_usr_key(int para) {
    UserKeyTable *ptr = UserKeyTable::Instance;
    if (ptr->isLoad) {
        if (ptr->loadKeyTab()) {
            return nullptr;
        } else {
            ptr->isLoad = 1;
        }
    }
    return ptr->getKey(para);
}


void UserKeyNode::operator<<(const YAML::Node &node) {
    this->userId = node["userId"].as<int>();
    this->userName = node["userName"].as<std::string>();
    this->userPasswdHash = node["userPasswdHash"].as<std::string>();

    for (int i = 0; i < 30; ++i) {
        this->tableIndex[i][0] = node["tableIndex"][i][0].as<int>();
        this->tableIndex[i][1] = node["tableIndex"][i][1].as<int>();
    }
    for (int i = 0; i < 30; ++i) {
        this->sortHave[i] = node["sortHave"][i].as<char>();
    }
    this->mask = node["mask"].as<unsigned int>();

}

void UserKeyNode::operator>>(YAML::Node &node) const {
    node["userId"] = this->userId;
    node["userName"] = this->userName;
    node["userPasswdHash"] = this->userPasswdHash;
    for (int i = 0; i < 30; ++i) {
        node["tableIndex"][i][0] = this->tableIndex[i][0];
        node["tableIndex"][i][1] = this->tableIndex[i][1];
    }
    for (int i = 0; i < 30; ++i) {
        node["sortHave"][i] = this->sortHave[i];
    }

    node["mask"] = this->mask;

}

std::string UserKeyNode::getFinalUserKay() const {
    char buff[128]{};
    for (int i = 0; i < 30; ++i) {
        buff[i] = randCharTab[tableIndex[i][0]][tableIndex[i][1]];
    }
    for(size_t i =30;i<strlen(getMainPasswd());++i){
        buff[i]=*(getMainPasswd()+i);
    }
    buff[strlen(getMainPasswd())]='\0';
    return std::string(buff);
}


std::string UserKeyTable::getKey(int id) const {
    for (const auto &n:tab) {
        if (n.second.userId == id) {
            return n.second.getFinalUserKay();
        }
    }
    return nullptr;
}

bool UserKeyTable::insertKey(const UserKeyNode &node) {
    return tab.insert(std::make_pair(node.userName, node)).second;
}

void UserKeyTable::operator<<(const YAML::Node &node) {
    for (auto n:node) {
        UserKeyNode iNode;
        iNode << n;
        tab.insert(std::make_pair(iNode.userName, iNode));
    }
}

void UserKeyTable::operator>>(YAML::Node &node) const {
    for (const auto &n:tab) {
        YAML::Node tmp;
        n.second >> tmp;
        node.push_back(tmp);
    }
}

int UserKeyTable::loadKeyTab() {
    LockGround lockGround(&lock);
    char userKeyFileStr[64]{};
    sprintf(userKeyFileStr, "%s/userKey.data", USER_KEY_PREFIX);

    namespace bf=boost::filesystem;
    bf::path userKeyFile(userKeyFileStr);
    bf::ifstream infile(userKeyFile);
    if (!infile) {
        INFO("key table not exists ,use null :" << userKeyFileStr)
        return 0;
//        return ERROR_IN;
    }
    size_t buffLen = bf::file_size(userKeyFile) * 3;
    char *buff = (char *) malloc(buffLen);
    char *decBuff = (char *) malloc(buffLen);

    size_t dataLen = 0;
    while (!infile.eof()) {
        infile.read(buff + dataLen, buffLen);
        dataLen += infile.gcount();
    }
    if (aesDataToData(getMainPasswd(), buff, dataLen, decBuff, buffLen, 0)) {
        ERROR_LOG("dec index fail :" << userKeyFileStr)
        free(buff);
        free(decBuff);
        return ERROR_IN;
    }
    decBuff[buffLen] = '\0';
//    DEBUG("yaml dec is" << decBuff)

    YAML::Node root = YAML::Load(decBuff);
    tab.clear();
    *Instance << root;
    free(buff);
    free(decBuff);
    return 0;
}

int UserKeyTable::savrKeyTab() {
    LockGround lockGround(&lock);
    //输出字符串
    YAML::Node node;
    *Instance >> node;
//    DEBUG("before enc : " << node)
    std::ostringstream outString;
    outString << node;
    std::string yamlNode = outString.str();
//    DEBUG("yaml origin is" << yamlNode)
    size_t outLen = yamlNode.length() * 10;
    char *decBuff = (char *) malloc(outLen);

    //加密
    if (aesDataToData(getMainPasswd(), yamlNode.c_str(), yamlNode.length(), decBuff, outLen, 1)) {
        ERROR_LOG("enc index fail ")
        free(decBuff);
        return ERROR_IN;
    }
    decBuff[outLen] = '\0';
    //保存
    namespace bf=boost::filesystem;

    char userKeyFileStr[64]{};
    sprintf(userKeyFileStr, "%s/userKey.data", USER_KEY_PREFIX);

    bf::ofstream outfile(userKeyFileStr, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!outfile) {
        ERROR_LOG("file don't exists :" << userKeyFileStr)
        free(decBuff);
        return ERROR_IN;
    }
    outfile.write(decBuff, outLen);
    free(decBuff);
    return 0;
}

int UserKeyTable::checkLogin(std::string name, std::string passed) {
    auto iterator = tab.find(name);
    if (iterator == tab.end()) {
        return 10;
    }

    HashAdaptor adaptor(ADAPTOR_MODE_STRING, passed);
    if(adaptor.encode(ADAPTOR_CRYPT_MD5)->isWrong()){
        ERROR_LOG("err when md5 user key")
        return ERROR_IN;
    }

    if(iterator->second.userPasswdHash!=adaptor.getResult()){
        ERROR_LOG("passwd is wrong")
        DEBUG("required asswdHash :"<<iterator->second.userPasswdHash<<"\t but get is "<<adaptor.getResult())
        return -9;
    }
    return iterator->second.userId;

}

int UserKeyTable::registor(std::string &name, std::string &passwd) {
    auto iterator = tab.find(name);
    int _id{};

    if(iterator!=tab.end()){
        ERROR_LOG("user  already exsts")
        return -7;
    }

    UserKeyNode userKeyNode{};
    do {
        _id = rand();
        for(auto &i:tab){
            if(i.second.userId==_id){
                _id=-1;
                break;
            }
        }
    } while (_id<0);
    userKeyNode.userId=tab.size()+1;

    userKeyNode.userName=name;

    HashAdaptor adaptor(ADAPTOR_MODE_STRING, passwd);

    if(adaptor.encode(ADAPTOR_CRYPT_MD5)->isWrong()){
        ERROR_LOG("err when md5 passwd")
        return ERROR_IN;
    }
    userKeyNode.userPasswdHash=adaptor.getResult();

    for(auto & i : userKeyNode.tableIndex){
        i[0]=rand()%USER_KEY_INDEX_ROW;
        i[1]=rand()%MAM_LINE_LEN;
    }

    for(auto & i :userKeyNode.sortHave){
        i=randChar();
    }
    userKeyNode.mask=rand();

    tab.insert(std::make_pair(userKeyNode.userName,userKeyNode));
    return userKeyNode.userId;
}
int UserKeyTable::randKey(){
    return rand()%tab.size();
}
