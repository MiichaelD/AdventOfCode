import re
reg_ex = "byr,(19[2-9][0-9]|200[012]);iyr,20(1[0-9]|20);eyr,20(2[0-9]|30);hgt,(1([5-8][0-9]|9[0-3])cm|(59|6[0-9]|7[0-6])in);hcl,#[0-9a-f]{6};ecl,(amb|blu|brn|gry|grn|hzl|oth);pid,[0-9]{9}".split(";")
print(
    sum(
        (len(p)-('cid' in p) == 7) + 1j *
        all(
            re.match('^'+v+'$', p.get(k, ""))
            for k, v in [
                x.split(",") for x in reg_ex
            ]
        )
        for p in [
            dict(
                v.split(':') for v in p.split()
            ) for p in open('2020/AdventOfCode_04_input.txt').read().split("\n\n")
        ]
    )
)
