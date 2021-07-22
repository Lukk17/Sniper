class NumericValidator {
  static String? validate(String? value, {num? max, num? min, bool? integer}) {
    if (value == null || value.isEmpty) {
      return "Input is empty";
    }

    if (max != null) {
      String? result = maxValidation(value, max);
      if (result != null) return result;
    }

    if (min != null) {
      String? result = minValidation(value, min);
      if (result != null) return result;
    }

    if (integer != null && integer) {
      String? result = integerValidation(value);
      if (result != null) return result;
    }
  }

  static String? integerValidation(value) {
    return _isInt(value) ? "This is not a integer !" : null;
  }

  static String? maxValidation(value, num max) {
    if (_isNum(value)) {
      value = num.parse(value);

      if (value > max) {
        return "Input can't be larger than $max";
      }
    } else {
      return "This is not a number !";
    }
  }

  static String? minValidation(value, num min) {
    if (_isNum(value)) {
      value = num.parse(value);

      if (value < min) {
        return "Input can't be less than $min";
      }
    } else {
      return "This is not a number !";
    }
  }

  static bool _isInt(value) {
    int? parsed = int.tryParse(value);
    return parsed == null || parsed.isNaN ? true : false;
  }

  static bool _isNum(value) {
    num? parsed = num.tryParse(value);
    return parsed == null || parsed.isNaN ? false : true;
  }
}
