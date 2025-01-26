use testing::sploosh;
#[test]
pub fn test_5() {
    assert_eq!(
        sploosh(sploosh(-1, 0, 0), sploosh(1, 1, 0), sploosh(3, 2, 0)),
        4
    );
}
